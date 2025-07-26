/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_pipe_helpers.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekhallaf <ekhallaf@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 05:17:32 by ekhallaf          #+#    #+#             */
/*   Updated: 2025/07/26 05:32:52 by ekhallaf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int create_pipe_if_needed(t_command *cmd, int pipefd[2])
{
    if (cmd->pipe)
    {
        if (pipe(pipefd) == -1)
        {
            perror("pipe");
            exit_status(1);
            return -1;
        }
    }
    return 0;
}

void    child_process(t_command *cmd, int prev_pipe, int pipefd[2], t_envp *env)
{
    if (prev_pipe != -1)
    {
        dup2(prev_pipe, STDIN_FILENO);
        close(prev_pipe);
    }
    if (cmd->pipe)
    {
        close(pipefd[0]);
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[1]);
    }
    setup_redirections(cmd);
    exec_command(cmd, env);
    exit(1);
}

int parent_pipe_cleanup(int prev_pipe, t_command *cmd, int pipefd[2])
{
    if (prev_pipe != -1)
        close(prev_pipe);

    if (cmd->pipe)
    {
        close(pipefd[1]);
        return pipefd[0];
    }
    return -1;
}

