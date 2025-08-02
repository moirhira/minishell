/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekhallaf <ekhallaf@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 23:01:03 by ekhallaf          #+#    #+#             */
/*   Updated: 2025/08/02 22:58:00 by ekhallaf         ###   ########.fr       */
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

int handle_builtin_if_no_pipe(t_command *cmd, t_envp **env)
{
    int status;
    if (is_builtin(cmd->args[0]) && cmd->pipe == 0)
    {
        status = execute_builtin(cmd, env);
        exit_status(status);
        return (status);
    }
    return -1; 
}

int	execute_pipeline(t_command *cmd_list, t_envp *env)
{
	int		result;
	pid_t	last_pid;

	result = handle_builtin_if_no_pipe(cmd_list, &env);
	if (result >= 0)
		return (result);
	setup_signals(SHELL_EXECUTING);
	last_pid = run_pipeline_loop(cmd_list, env);
	if (last_pid == -1)
		return (1);
	result = wait_children(last_pid);
	setup_signals(SHELL_INTERACTIVE);
	return (result);
}

