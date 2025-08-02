/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moirhira <moirhira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 23:01:03 by ekhallaf          #+#    #+#             */
/*   Updated: 2025/08/02 11:04:24 by moirhira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

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

static int wait_children(pid_t last_pid)
{
    int status;
    int last_exit_status = 0;
    pid_t wpid;

    while ((wpid = waitpid(-1, &status, 0)) > 0)
    {
        if (wpid == last_pid)
        {
            if (WIFEXITED(status))
                last_exit_status = WEXITSTATUS(status);
            else if (WIFSIGNALED(status))
            {
                last_exit_status = 128 + WTERMSIG(status);
                if (WTERMSIG(status) == SIGINT)
                    write(STDERR_FILENO, "\n", 1);
                if (WTERMSIG(status) == SIGQUIT)
                    write(STDOUT_FILENO, "Quit (core dumped)\n", 20);
            }
        }
    }
    return (last_exit_status);
}


int execute_pipeline(t_command *cmd_list, t_envp *env)
{
    int prev_pipe = -1;
    int pipefd[2];
    pid_t pid;
    pid_t last_pid;
    t_command *cmd = cmd_list;
    int result;
    
    last_pid = -1;

    setup_signals(SHELL_EXECUTING);
    while (cmd)
    {
        if (create_pipe_if_needed(cmd, pipefd) == -1)
            return 1;

        pid = fork();
        if (pid == -1)
        {
            perror("fork");
            exit_status(1);
            return 1;
        }
        if (pid == 0)
        {
            setup_signals(CHILD_PROCESS);
            child_process(cmd, prev_pipe, pipefd, env);
        }
        if (!cmd->next)
            last_pid = pid;
        prev_pipe = parent_pipe_cleanup(prev_pipe, cmd, pipefd);
        cmd = cmd->next;
    }
    result = wait_children(last_pid);
    setup_signals(SHELL_INTERACTIVE);
    return (result);
}

