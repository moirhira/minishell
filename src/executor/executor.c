/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moirhira <moirhira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 21:07:51 by moirhira          #+#    #+#             */
/*   Updated: 2025/07/22 09:31:59 by moirhira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#include "../../include/minishell.h"
volatile sig_atomic_t g_signal_received = 0;




int execute_builtin()
{
    return (-1);
} 

int execute_external(t_command *command, t_envp *env)
{
    return (0);
}

int execute_commands(t_command *command, t_envp **my_env)
{
    int sts = 0;
    parse_heredocs(command, (*my_env));
    
    if (g_signal_received)
        return (exit_status(-1));
    
    int res = execute_builtin();
    if (res >= 0)
        return (exit_status(-1));
    sts = execute_external(command, (*my_env));
        
    return (exit_status(sts));
}