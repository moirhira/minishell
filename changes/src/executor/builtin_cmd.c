/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cmd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekhallaf <ekhallaf@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 22:51:41 by ekhallaf          #+#    #+#             */
/*   Updated: 2025/07/27 22:52:05 by ekhallaf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	save_std_fds(int *stdin_fd, int *stdout_fd)
{
	*stdin_fd = dup(STDIN_FILENO);
	*stdout_fd = dup(STDOUT_FILENO);
	if (*stdin_fd == -1 || *stdout_fd == -1)
	{
		perror("dup failed");
		close(*stdin_fd);
		close(*stdout_fd);
		return (0);
	}
	return (1);
}

static int	restore_std_fds(int stdin_fd, int stdout_fd)
{
	int	ret;

	ret = 1;
	if (dup2(stdin_fd, STDIN_FILENO) == -1
		|| dup2(stdout_fd, STDOUT_FILENO) == -1)
	{
		perror("failed to restore file descriptor");
		ret = 0;
	}
	close(stdin_fd);
	close(stdout_fd);
	return (ret);
}

static int	run_builtin_command(t_command *cmd, t_envp **env)
{
	if (strcmp(cmd->args[0], "cd") == 0)
		return (builtin_cd(cmd, env));
	if (strcmp(cmd->args[0], "pwd") == 0)
		return (builtin_pwd(*env));
	if (strcmp(cmd->args[0], "echo") == 0)
		return (builtin_echo(cmd->args));
	if (strcmp(cmd->args[0], "env") == 0)
		return (builtin_env(*env));
	if (strcmp(cmd->args[0], "export") == 0)
		return (builtin_export(cmd, env));
	if (strcmp(cmd->args[0], "unset") == 0)
		return (builtin_unset(cmd->args, env));
	if (strcmp(cmd->args[0], "exit") == 0)
		return (builtin_exit(cmd->args, env));
	return (-1);
}

int	execute_builtin(t_command *cmd, t_envp **env)
{
	int		result;
	int		s_stdin;
	int		s_stdout;

	s_stdin = -1;
	s_stdout = -1;
	if (cmd->redirects)
		if (!save_std_fds(&s_stdin, &s_stdout))
			return (1);
	setup_redirections(cmd);
	result = run_builtin_command(cmd, env);
	if (cmd->redirects)
		restore_std_fds(s_stdin, s_stdout);
	return (result);
}
