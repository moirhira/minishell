/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moirhira <moirhira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 21:07:51 by moirhira          #+#    #+#             */
/*   Updated: 2025/07/15 10:37:10 by moirhira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../../include/minishell.h"


void parse_heredocs(t_command *command)
{
    t_command *cmd = command;
    while (cmd)
    {
        if (cmd->heredoc_count > 0)
        {
            ;
        }
        cmd = cmd->next;
    }
    
}


int execute_builtin()
{
    return (0);
} 

int execute_external()
{
    return (0);
}
int execute_commands(t_command *command)
{
    parse_heredocs(command);
    int res = execute_builtin();
    if (res >= 0)
    {
        //free
        return (res);
    }
    execute_external();
    return (0);
}