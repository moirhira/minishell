/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moirhira <moirhira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 21:07:57 by moirhira          #+#    #+#             */
/*   Updated: 2025/04/27 22:36:10 by moirhira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void tokinisition(t_token **token, char *command, char **my_env)
{
    if (!split_token(command, my_env, token))
    {
        free_arr(my_env);
        free_token(token);
        exit(EXIT_FAILURE);
    }
}

void parse_command(t_token **token_list, char *cmd_line, char **my_env)
{
    tokinisition(token_list, cmd_line, my_env);
    t_token *ptr;
    ptr = *token_list;
    while (ptr)
    {
        printf("value : %s\n", ptr->value);
        printf("type : %d\n", ptr->type);
        printf("attached : %d\n", ptr->attached);
        printf("....................................\n");
        ptr = ptr->next;
    }
    // parsing();
}