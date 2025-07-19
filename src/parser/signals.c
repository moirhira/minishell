/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekhallaf <ekhallaf@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 02:37:52 by ekhallaf          #+#    #+#             */
/*   Updated: 2025/07/19 13:55:11 by ekhallaf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//  #include "../../include/minishell.h"

// void sig_handler_inter(int sig)
// {
//     if (sig == SIGINT)
//     {
//         write(STDOUT_FILENO, "\n", 1);
//         rl_on_new_line();
//         rl_replace_line("", 0);
//         rl_redisplay();
//         exit_status(130);
//     }
// }

// void sig_handler_execution(int sig)
// {
//     if (sig == SIGINT)
//     {
//         write(STDOUT_FILENO, "\n", 1);
//         exit_status(130);
//     }
//     if (sig == SIGQUIT)
//     {
//         write(STDOUT_FILENO, "Quit (core dumped)\n", 20);
//         exit_status(131);
//     }
// }

// void    setup_signals(int state)
// {
//     if (state == SHELL_EXECUTING)
//     {
//         signal(SIGINT, sig_handler_execution);
//         signal(SIGQUIT, sig_handler_execution);
//     }
//     else if (state == SHELL_INTERACTIVE)
//     {
//         signal(SIGINT, sig_handler_inter);
//         signal(SIGQUIT, SIG_IGN);
//     }
//     else if (state == SHELL_HEREDOC)
//     {
//         signal(SIGINT, SIG_IGN);
//         signal(SIGQUIT, SIG_IGN);
//     }
// }
