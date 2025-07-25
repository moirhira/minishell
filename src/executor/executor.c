/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moirhira <moirhira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 11:09:06 by ekhallaf          #+#    #+#             */
/*   Updated: 2025/07/25 15:27:25 by moirhira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
volatile sig_atomic_t g_signal_received = 0;

int execute_commands(t_command *command, t_envp **env)
{
    int exit_st = 0;
    parse_heredocs(command, (*env));
   
    if (g_signal_received)
    {
        write(STDOUT_FILENO, "\n", 1);
        g_signal_received = 0;
        return (exit_status(130));
    }
    if (command && command->next == NULL)
    {
        if (!command->args || !command->args[0])
            return 0;
        exit_st = execute_builtin(command,env);
        if (exit_st >= 0)
            return (exit_status(exit_st));
        exit_st = execute_external(command, *env);
        return (exit_status(exit_st));
    }
    else if (command && command->pipe)
    {
        execute_pipeline(command, *env);
        return (exit_status(exit_st));
    }
    return (0);
}
