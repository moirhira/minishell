/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moirhira <moirhira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 21:07:38 by moirhira          #+#    #+#             */
/*   Updated: 2025/08/05 19:07:36 by moirhira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
#define MINISHELL_H
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <unistd.h>
#include <sys/stat.h>
#include <limits.h>
#include <string.h>
#include "../libraries/libft/libft.h"
#include <linux/limits.h>
#include <fcntl.h> 
#include <errno.h>
#include <sys/wait.h>


extern volatile sig_atomic_t g_signal_received;
#define SIZE_ENV 1024

typedef enum e_shell_state
{
    SHELL_INTERACTIVE = 1,
    SHELL_EXECUTING = 2,
    SHELL_HEREDOC = 3,
    CHILD_PROCESS = 4
}   t_shell_state;

typedef enum e_token_type {
    TOKEN_WORD,       // word
    TOKEN_PIPE,        // |
    TOKEN_INPUT,       // <
    TOKEN_OUTPUT,     // >
    TOKEN_APPEND,      // >>
    TOKEN_HEREDOC,     // <<
    TOKEN_HEREDOC_QUOTED, 
    TOKEN_EOL        // end of
} t_token_type;

typedef struct s_token {
    t_token_type type;
    char *value;
    int attached;
    int was_quoted;
    int ignored;
    struct s_token *next;
}   t_token;

typedef struct s_redirect
{
    char *filename;
    int type;
    char *content;
    struct s_redirect *next;
}   t_redirect;

typedef struct s_command
{
    char **args; 
    int heredoc_count;
    int infile_count;
    int outfile_count;
    int append_count; 
    int pipe;
    struct s_redirect *redirects;
    struct s_command *next;
}   t_command;

typedef struct s_envp
{
    char *key;
    char *value;
    struct s_envp *next;
}   t_envp;

// tokenizer.c
int split_token(char *s, t_envp **my_env, t_token **token);
char *handel_env_var(char *s, int *i, t_envp **my_env, char *curnt_str);

// tokenizer_utils.c
t_token *create_token(char *str, int type, int is_attached, int was_quoted);
void add_token(t_token **token_lst, t_token *new_token);
char *get_env_value(t_envp *my_env, const char *var_name); // get the value of the env vars 
int was_previous_space(char *s, int i);
t_token *get_last_token(t_token *lst);

// parse.c
int parse_command(t_token **token_list, t_command **token_lst, char *cmd_line, t_envp **my_env);

//parser_utils.c
int handel_pipe(t_token **token, t_command **head, t_command **cmd_lst);
int handel_input_redirection(t_token **token, t_command *head);
int handel_output_redirection(t_token **token, t_command *head);
int handel_append_redirection(t_token **token, t_command *head);
int handel_heredoc(t_token **token, t_command *head);
void handel_argument(t_token **token, t_command *head);

//parser_utils_1.c
t_command *creat_command(void);
void add_command(t_command **command_lst, t_command *new_command);
void add_argument(t_command *cmd, char *arg);
int check_next_token(t_token *token, t_command *head);
void  add_redirect(t_command *cmd, int type, const char *filename);

// utils.c
void	free_token(t_token **stacka);
void	free_env(t_envp **env);
void	free_command(t_command **command);
int     exit_status(int new_status);


// dispal_error.c
void	display_error(char *command_or_file, char *error_message);


// env_oprations.c
int  update_env_var(t_envp **env, char *key, char *value);


// execution.c
int     exit_status(int new_status);
int     execute_commands(t_command *command, t_envp **env);
int     execute_builtin(t_command *cmd, t_envp **env);
int    builtin_cd(t_command *cmd, t_envp **env);
char *find_command_in_path(char *cmd, t_envp *env, int *status);
void    ft_swap(char **a, char **b);
void    free_env_array(char **envp);

// parse_herdocs.c
void parse_heredocs(t_command *command, t_envp *my_env);


// setup_signals.c
void    setup_signals(int state);

// utils_2.c
int only_whitespace(char *str);
char **convert_env_to_array(t_envp *env);
int	ft_lstsize(t_envp *lst);
int has_input_redir(t_command *cmd);
int has_output_redir(t_command *cmd);

// for export 
int     print_sorted_export(t_envp *env);
int     is_valid_identifier(const char *str);
int     builtin_export(t_command *cmd, t_envp **env);
int     count_env(t_envp *env);
void    sort_in_tab(char **array, int size);
int     print_sorted_export(t_envp *env);

// unset
int    builtin_unset(char **args, t_envp **env);

// exit
int	    builtin_exit(char **args, t_envp **env);

// echo
int    builtin_echo(char **cmd);

// helper
int is_valid_identifier(const char *str);

//pwd
int	builtin_pwd(t_envp *env);

// env
int   builtin_env(t_envp *envp);


// helper 2
void    sort_in_tab(char **array, int size);
int     count_env(t_envp *env);
int	    print_sorted_export(t_envp *env);
char	*create_export_entry(char *key, char *value);
void	print_array(char **arr);


// execute single command 
int        execute_external(t_command *cmd, t_envp *env);
int        is_builtin(const char *cmd);

// setup_redirctions 
int setup_redirections(t_command *cmd, int exit_or_return);
int execute_pipeline(t_command *cmd_list, t_envp *env);




// pipeline
int exec_command(t_command *cmd, t_envp *env);
int execute_pipeline(t_command *cmd_list, t_envp *env);
int create_pipe_if_needed(t_command *cmd, int pipefd[2]);
void child_process(t_command *cmd, int prev_pipe, int pipefd[2], t_envp *env);
int parent_pipe_cleanup(int prev_pipe, t_command *cmd, int pipefd[2]);


#endif

