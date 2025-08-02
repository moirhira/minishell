/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_helpers.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekhallaf <ekhallaf@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 21:12:46 by ekhallaf          #+#    #+#             */
/*   Updated: 2025/08/02 21:14:02 by ekhallaf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../../include/minishell.h"

static int	spawn_command(t_command *cmd, t_envp *env,
							int prev_pipe, int pipefd[2])
{
	pid_t	pid;

	if (create_pipe_if_needed(cmd, pipefd) == -1)
		return (-1);
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		exit_status(1);
		return (-1);
	}
	if (pid == 0)
	{
		setup_signals(CHILD_PROCESS);
		child_process(cmd, prev_pipe, pipefd, env);
	}
	return (pid);
}

int	run_pipeline_loop(t_command *cmd, t_envp *env)
{
	int		prev_pipe;
	int		pipefd[2];
	pid_t	last_pid;

	prev_pipe = -1;
	last_pid = -1;
	while (cmd)
	{
		pid_t pid = spawn_command(cmd, env, prev_pipe, pipefd);
		if (pid == -1)
			return (-1);
		if (!cmd->next)
			last_pid = pid;
		prev_pipe = parent_pipe_cleanup(prev_pipe, cmd, pipefd);
		cmd = cmd->next;
	}
	return (last_pid);
}
