/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moirhira <moirhira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 11:09:06 by ekhallaf          #+#    #+#             */
/*   Updated: 2025/07/16 10:01:26 by moirhira         ###   ########.fr       */
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
int execute_external()
{
    return (0);
}
int execute_commands(t_command *command, t_envp **env)
{
    // parse_heredocs(command);
    int res = execute_builtin(command,env);
    if (res >= 0)
    {
        //free
        return (res);
    }
    execute_external();
    return (0);
}
