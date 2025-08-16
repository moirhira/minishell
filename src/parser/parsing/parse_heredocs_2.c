/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_heredocs_2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moirhira <moirhira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 14:46:55 by moirhira          #+#    #+#             */
/*   Updated: 2025/08/16 08:59:56 by moirhira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

static char	*expand_variabels(char *line, t_envp *my_env)
{
	char	ch[2];
	int		i;
	char	*processed_line;

	processed_line = ft_calloc(1, sizeof(char));
	i = 0;
	while (line[i])
	{
		if (line[i] == '$')
			processed_line = handel_env_var(line, &i, &my_env, processed_line);
		else
		{
			ch[0] = line[i];
			ch[1] = '\0';
			processed_line = ft_strjoin(processed_line, ch);
			i++;
		}
	}
	return (processed_line);
}

void	read_from_heredoc(int fd, char *line, t_envp *my_env, int expand_var)
{
	char	*processed_line;

	if (expand_var)
	{
		processed_line = expand_variabels(line, my_env);
		ft_putstr_fd(processed_line, fd);
		ft_putstr_fd("\n", fd);
	}
	else
	{
		ft_putstr_fd(line, fd);
		ft_putstr_fd("\n", fd);
	}
}

void	process_child(char *delimiter, int expnad, t_envp *env, int fd)
{
	char	*line;

	setup_signals(SHELL_HEREDOC);
	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			ft_putstr_fd("warning: here-document delimited by EOF (wanted `",
				2);
			ft_putstr_fd(delimiter, 2);
			ft_putstr_fd("')\n", 2);
			break ;
		}
		if (ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
		}
		read_from_heredoc(fd, line, env, expnad);
		free(line);
	}
	close(fd);
	free_all_memory();
	exit(EXIT_SUCCESS);
}

int	wait_for_child(pid_t pid, char *temp_filename)
{
	int	status;

	waitpid(pid, &status, 0);
	setup_signals(SHELL_INTERACTIVE);
	if (WIFEXITED(status))
	{
		if (WEXITSTATUS(status) == EXIT_SUCCESS)
			return (1);
		else if (WEXITSTATUS(status) == 130)
		{
			g_signal_received = SIGINT;
			unlink(temp_filename);
			return (0);
		}
		else
		{
			g_signal_received = WEXITSTATUS(status);
			unlink(temp_filename);
			return (0);
		}
	}
	return (1);
}

int	fill_herdoc(char *delimiter, int expand_var, t_envp *my_env,
		char *temp_filename)
{
	int		fd;
	pid_t	pid;

	fd = open(temp_filename, O_WRONLY | O_CREAT | O_TRUNC, 0600);
	if (fd == -1)
		return (0);
	fd_collector(fd, 0);
	setup_signals(SHELL_IGNORE);
	pid = fork();
	if (pid == -1)
	{
		close(fd);
		return (0);
	}
	if (pid == 0)
		process_child(delimiter, expand_var, my_env, fd);
	else
	{
		close(fd);
		return (wait_for_child(pid, temp_filename));
	}
	return (0);
}
