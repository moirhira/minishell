/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moirhira <moirhira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 21:07:38 by moirhira          #+#    #+#             */
/*   Updated: 2025/04/28 15:36:38 by moirhira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
#define MINISHELL_H
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "../libraries/libft/libft.h"

typedef enum e_token_type {
    TOKEN_WORD,        // word
    TOKEN_PIPE,        // |
    TOKEN_INPUT,       // <
    TOKEN_OUTPUT,      // >
    TOKEN_APPEND,      // >>
    TOKEN_HEREDOC,     // <<
    TOKEN_EOL        // end of
} t_token_type;

typedef struct s_token {
    t_token_type type;
    char *value;
    int attached;
    struct s_token *next;
} t_token;


typedef struct s_command
{
    char **args;    // command argument
    char *infile;   // fro < 
    char *outfile;  // for > or >>
    int append;     // 1 if >>
    int pipe;       // 1 if followed by |
    char *herdoc;   // for <<
    struct s_command *next;
} t_command;

// tokenizer.c
t_token *split_token(char *s, char **my_env, t_token **token);

// tokenizer_utils.c
t_token *create_token(char *str, int type, int is_attached);
void add_token(t_token **token_lst, t_token *new_token);
char *get_env_value(char **my_env, const char *var_name);
int was_previous_space(char *s, int i);


// parse.c
void parse_command(t_token **token_list, t_command **token_lst, char *cmd_line, char **my_env);

// utils.c
void	free_token(t_token **stacka);
char	*free_arr(char **res);
void	free_command(t_command **command);
#endif