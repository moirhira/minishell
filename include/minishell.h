/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moirhira <moirhira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 21:07:38 by moirhira          #+#    #+#             */
/*   Updated: 2025/08/17 09:56:25 by moirhira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
# include "../libraries/libft/libft.h"
# include <errno.h>
# include <fcntl.h>
# include <limits.h>
# include <linux/limits.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <unistd.h>

extern volatile sig_atomic_t	g_signal_received;

typedef enum e_shell_state
{
	SHELL_INTERACTIVE = 1,
	SHELL_IGNORE = 2,
	SHELL_HEREDOC = 3,
	CHILD_PROCESS = 4
}								t_shell_state;

typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_INPUT,
	TOKEN_OUTPUT,
	TOKEN_APPEND,
	TOKEN_HEREDOC,
	TOKEN_HEREDOC_QUOTED,
	TOKEN_EOL
}								t_token_type;

typedef struct s_token
{
	t_token_type				type;
	char						*value;
	int							attached;
	int							was_quoted;
	int							ignored;
	struct s_token				*next;
}								t_token;

typedef struct s_redirect
{
	char						*filename;
	int							type;
	struct s_redirect			*next;
}								t_redirect;

typedef struct s_command
{
	char						**args;
	int							heredoc_count;
	int							pipe;
	struct s_redirect			*redirects;
	struct s_command			*next;
}								t_command;

typedef struct s_envp
{
	char						*key;
	char						*value;
	struct s_envp				*next;
}								t_envp;

// tokenizer
int								split_token(char *s, t_envp **my_env,
									t_token **token);
char							*handel_env_var(char *s, int *i,
									t_envp **my_env, char *curnt_str);
int								handel_quoted_str(char *s, int i,
									t_envp **my_env, t_token **token);
int								handel_simple_str(char *s, int i,
									t_envp **my_env, t_token **token);
char							*get_var_value_and_advance(char *s, int *i,
									t_envp **my_env, int *q);
int								handel_heredoc_delimiter(char *s, int i,
									t_token **token, int *state);
int								handel_operator(char *s, int i, t_token **token,
									int *state);
t_token							*create_token(char *str, int type,
									int is_attached, int was_quoted);
void							add_token(t_token **token_lst,
									t_token *new_token);
char							*get_env_value(t_envp *my_env,
									const char *var_name);
int								should_attach_token(char *s, int i);
t_token							*get_last_token(t_token *lst);

// parser
int								parse_command(t_token **token_list,
									t_command **token_lst, char *cmd_line,
									t_envp **my_env);
int								handel_pipe(t_token **token, t_command **head,
									t_command **cmd_lst);
int								handel_input_redirection(t_token **token,
									t_command *head);
int								handel_output_redirection(t_token **token,
									t_command *head);
int								handel_append_redirection(t_token **token,
									t_command *head);
int								handel_heredoc(t_token **token,
									t_command *head);
int								handel_argument(t_token **token,
									t_command *head);
t_command						*creat_command(void);
void							add_command(t_command **command_lst,
									t_command *new_command);
int								add_argument(t_command *cmd, char *arg);
void							add_redirect(t_command *cmd, int type,
									const char *filename);
int								validate_syntaxe(t_token **token_lst);
int								parse_heredocs(t_command *command,
									t_envp *my_env);
int								fill_herdoc(char *delimiter, int expand_var,
									t_envp *my_env, char *temp_filename);

// execution
int								update_env_var(t_envp **env, char *key,
									char *value);
t_envp							*new_env_node(char *key, char *value);
void							append_env(t_envp **env, t_envp *node);
int								exit_status(int new_status);
int								execute_commands(t_command *command,
									t_envp **env);
int								execute_builtin(t_command *cmd, t_envp **env);
int								builtin_cd(t_command *cmd, t_envp **env);
char							*find_command_in_path(char *cmd, t_envp *env,
									int *status);
char							*ft_strjoin_path(const char *dir,
									const char *cmd);
int								is_file_exists(const char *path);
int								is_executable(const char *path);
int								is_directory(const char *path);
int								catch_execve_fail(t_command *cmd, char **envp,
									char *path);

// utils
int								only_whitespace(char *str);
char							**convert_env_to_array(t_envp *env);
int								ft_lstsize(t_envp *lst);
int								has_input_redir(t_command *cmd);
int								has_output_redir(t_command *cmd);
int								exit_status(int new_status);
void							display_error(char *command_or_file,
									char *error_message);
void							setup_signals(int state);
void							fd_collector(int fd, int mode);

// main.utils.c
int								only_whitespace(char *str);
char							*read_input(void);
t_envp							*retrieve_envp(char **env);

// builtin
int								print_sorted_export(t_envp *env);
int								handle_export_arg(char *arg, t_envp **env);
int								builtin_export(t_command *cmd, t_envp **env);
int								builtin_unset(char **args, t_envp **env);
int								builtin_exit(char **args);
long							my_strtol(const char *str, char **endptr,
									int base);
int								builtin_echo(char **cmd);
int								builtin_pwd(void);
int								builtin_env(t_envp *envp);
int								execute_external(t_command *cmd, t_envp *env);
int								is_builtin(const char *cmd);
int								setup_redirections(t_command *cmd,
									int exit_or_return);
int								execute_pipeline(t_command *cmd_list,
									t_envp *env);
int								exec_command(t_command *cmd, t_envp *env);
int								execute_pipeline(t_command *cmd_list,
									t_envp *env);
int								create_pipe_if_needed(t_command *cmd,
									int pipefd[2]);
void							child_process(t_command *cmd, int prev_pipe,
									int pipefd[2], t_envp *env);
int								parent_pipe_cleanup(int prev_pipe,
									t_command *cmd, int pipefd[2]);

#endif