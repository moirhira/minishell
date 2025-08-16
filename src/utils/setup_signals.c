/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_signals.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moirhira <moirhira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 09:14:12 by moirhira          #+#    #+#             */
/*   Updated: 2025/08/16 09:02:07 by moirhira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

volatile sig_atomic_t	g_signal_received = 0;

void	sig_handler_inter(int sig)
{
	if (sig == SIGINT)
	{
		g_signal_received = 128 + SIGINT;
		ft_putstr_fd("\n", STDERR_FILENO);
		exit_status(130);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

void	sig_handler_heredoc(int sig)
{
	(void)sig;
	ft_putstr_fd("\n", STDERR_FILENO);
	free_all_memory();
	fd_collector(-1, 1);
	exit(130);
}

void	setup_signals(int state)
{
	if (state == SHELL_INTERACTIVE)
	{
		signal(SIGINT, sig_handler_inter);
		signal(SIGQUIT, SIG_IGN);
	}
	else if (state == SHELL_IGNORE)
	{
		signal(SIGINT, SIG_IGN);
		signal(SIGQUIT, SIG_IGN);
	}
	else if (state == SHELL_HEREDOC)
	{
		signal(SIGINT, sig_handler_heredoc);
		signal(SIGQUIT, SIG_IGN);
	}
	else if (state == CHILD_PROCESS)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
	}
}
