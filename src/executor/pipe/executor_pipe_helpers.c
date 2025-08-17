/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_pipe_helpers.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moirhira <moirhira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 05:17:32 by ekhallaf          #+#    #+#             */
/*   Updated: 2025/08/16 22:55:42 by moirhira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

int	is_builtin(const char *cmd)
{
	int			i;
	const char	*builtins[] = {"echo", "export", "pwd", "cd", "unset", "env",
		"exit", NULL};

	i = 0;
	while (builtins[i])
	{
		if (ft_strcmp(cmd, builtins[i]) == 0)
			return (1);
		i++;
	}
	return (0);
}

int	catch_execve_fail(t_command *cmd, char **envp, char *path)
{
	char	*argv[3];

	if (errno == ENOENT)
	{
		display_error(cmd->args[0], "No such file or directory");
		return (127);
	}
	else if (errno == EACCES)
	{
		display_error(cmd->args[0], "Permission denied");
		return (126);
	}
	else if (errno == ENOEXEC)
	{
		argv[0] = "sh";
		argv[1] = path;
		argv[2] = NULL;
		execve("/bin/sh", argv, envp);
		display_error(cmd->args[0], "cannot execute binary file");
		return (126);
	}
	else
		return (display_error(cmd->args[0], strerror(errno)), 126);
}

int	exec_command(t_command *cmd, t_envp *env)
{
	char	*path;
	char	**envp;
	int		status;

	status = 0;
	path = find_command_in_path(cmd->args[0], env, &status);
	if (!path)
		return (status);
	envp = convert_env_to_array(env);
	if (!envp)
		return (1);
	execve(path, cmd->args, envp);
	status = catch_execve_fail(cmd, envp, path);
	return (status);
}

void	child_process(t_command *cmd, int prev_pipe, int pipefd[2], t_envp *env)
{
	int	exit_st;

	setup_signals(CHILD_PROCESS);
	exit_st = 0;
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
	setup_redirections(cmd, 1);
	if (cmd->args && cmd->args[0] && is_builtin(cmd->args[0]))
		exit_st = execute_builtin(cmd, &env);
	else if (cmd->args && cmd->args[0])
		exit_st = exec_command(cmd, env);
	free_all_memory();
	fd_collector(-1, 1);
	exit(exit_st);
}
