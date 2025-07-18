/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moirhira <moirhira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 21:07:51 by moirhira          #+#    #+#             */
/*   Updated: 2025/07/18 14:58:34 by moirhira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#include "../../include/minishell.h"
volatile sig_atomic_t g_signal_received = 0;




int execute_builtin()
{
    return (0);
} 

int execute_external()
{
    return (0);
}

void setup_redirections(t_command *cmd)
{
    t_redirect *redir = cmd->redirects;
    
    while (redir)
    {
        if (redir->type == TOKEN_HEREDOC || redir->type == TOKEN_HEREDOC_QUOTED)
        {
            int fd = open(redir->filename, O_RDONLY);
            if (fd == -1)
            {
                ft_putstr_fd("minishell: heredoc: Could not open temporary file\n", 2);
                exit(1);
            }
            dup2(fd, STDIN_FILENO);
            close(fd);
        }
        redir = redir->next;
    }
}
int execute_commands(t_command *command, t_envp **my_env)
{
    parse_heredocs(command, (*my_env));
    setup_redirections(command);
    int res = execute_builtin();
    if (res >= 0)
    {
        //free
        return (res);
    }
    execute_external();
    return (0);
}