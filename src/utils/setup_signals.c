/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_signals.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moirhira <moirhira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 09:14:12 by moirhira          #+#    #+#             */
/*   Updated: 2025/07/24 16:46:11 by moirhira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"


void sig_handler_inter(int sig)
{
    if (sig == SIGINT)
    {
        g_signal_received = 1;
        write(STDOUT_FILENO, "\n", 1);
        rl_on_new_line();
        rl_replace_line("", 0);
        rl_redisplay();
        exit_status(130);
    }
}

void sig_handler_execution(int sig)
{
    if (sig == SIGINT)
    {
        write(STDOUT_FILENO, "\n", 1);
        exit_status(130);
    }
    if (sig == SIGQUIT)
    {
        write(STDOUT_FILENO, "Quit (core dumped)\n", 20);
        exit_status(131);
    }
}

void    setup_signals(int state)
{
    write(STDOUT_FILENO, "\r", 1);
    if (state == SHELL_EXECUTING)
    {
        signal(SIGINT, sig_handler_execution);
        signal(SIGQUIT, sig_handler_execution);
    }
    else if (state == SHELL_INTERACTIVE)
    {
        signal(SIGINT, sig_handler_inter);
        signal(SIGQUIT, SIG_IGN);
    }
    else if (state == SHELL_HEREDOC)
    {
        signal(SIGINT, SIG_IGN);
        signal(SIGQUIT, SIG_IGN);
    }
}