/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moirhira <moirhira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 23:01:03 by ekhallaf          #+#    #+#             */
/*   Updated: 2025/08/12 21:07:25 by moirhira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

int parent_pipe_cleanup(int prev_pipe, t_command *cmd, int pipefd[2])
{
    if (prev_pipe != -1)
        close(prev_pipe);
    if (cmd->pipe)
    {
        close(pipefd[1]);
        return pipefd[0];
    }
    return (-1);
}

int create_pipe_if_needed(t_command *cmd, int pipefd[2])
{
    if (cmd->pipe)
    {
        if (pipe(pipefd) == -1)
        {
            display_error("pipe", strerror(errno));
            return (-1);
        }
        fd_collector(pipefd[0], 0);
        fd_collector(pipefd[1], 0);
    }
    return (0);
}

static int wait_children(pid_t last_pid)
{
    int status;
    int last_exit_status = 0;
    int sigint_rec;
    pid_t wpid;

    sigint_rec = 0;
    while ((wpid = waitpid(-1, &status, 0)) > 0)
    {
        if (WIFSIGNALED(status))
        {
            if (WTERMSIG(status) == SIGINT)
                sigint_rec = 1;
            if (wpid == last_pid)
            {
                last_exit_status = 128 + WTERMSIG(status);
                if (WTERMSIG(status) == SIGQUIT)
                    write(STDERR_FILENO, "Quit (core dumped)\n", 20);
            }
        }
        else if (WIFEXITED(status) && wpid == last_pid)
            last_exit_status = WEXITSTATUS(status);
    }
    if (sigint_rec)
        write(STDOUT_FILENO, "\n", 1);
    return (last_exit_status);
}


static pid_t run_pipeline(t_command *cmd, t_envp *env)
{
    pid_t last_pid;
    int prev_pipe;
    int pipefd[2];
    pid_t pid;
    
    last_pid = -1;
    prev_pipe = -1;
    while (cmd)
    {
        if (create_pipe_if_needed(cmd, pipefd) == -1)
            return (-1);
        pid = fork();
        if (pid == -1)
            return (display_error("fork", strerror(errno)), -1);
        if (pid == 0)
            child_process(cmd, prev_pipe, pipefd, env);
        if (!cmd->next)
            last_pid = pid;
        prev_pipe = parent_pipe_cleanup(prev_pipe, cmd, pipefd);
        cmd = cmd->next;
    }
    return (last_pid);
}

int execute_pipeline(t_command *cmd_list, t_envp *env)
{
    pid_t last_pid;
    int result;
    
    
    setup_signals(SHELL_IGNORE);
    last_pid = run_pipeline(cmd_list, env);
    if (last_pid == -1)
        return (1);
    result = wait_children(last_pid);
    setup_signals(SHELL_INTERACTIVE);
    return (result);
}



