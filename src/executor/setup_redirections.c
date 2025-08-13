/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_redirections.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moirhira <moirhira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 06:48:59 by ekhallaf          #+#    #+#             */
/*   Updated: 2025/08/13 21:33:42 by moirhira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	f_error(int fd, t_redirect *redir, int target_fd)
{
	if (fd == -1)
	{
		display_error(redir->filename, strerror(errno));
		return (0);
	}
	if (dup2(fd, target_fd) == -1)
	{
		ft_putstr_fd("minishell: dup2 failed\n", 2);
		close(fd);
		return (0);
	}
	close(fd);
	return (1);
}

static int	open_redir_file(t_redirect *redir, int flags, int mode,
		int exit_return)
{
	int	fd;
	int	target_file;

	if (flags & O_WRONLY)
		target_file = STDOUT_FILENO;
	else
		target_file = STDIN_FILENO;
	fd = open(redir->filename, flags, mode);
	if (!f_error(fd, redir, target_file))
	{
		if (exit_return)
		{
			fd_collector(-1, 1);
			free_all_memory();
			exit(exit_status(1));
		}
		return (1);
	}
	return (0);
}

int	setup_redirections(t_command *cmd, int exit_or_return)
{
	t_redirect	*redir;
	int			res;

	res = 0;
	redir = cmd->redirects;
	while (redir)
	{
		if (redir->type == TOKEN_HEREDOC_QUOTED || redir->type == TOKEN_HEREDOC
			|| redir->type == TOKEN_INPUT)
		{
			res = open_redir_file(redir, O_RDONLY, 0, exit_or_return);
		}
		else if (redir->type == TOKEN_OUTPUT)
			res = open_redir_file(redir, O_WRONLY | O_CREAT | O_TRUNC, 0644,
					exit_or_return);
		else if (redir->type == TOKEN_APPEND)
			res = (open_redir_file(redir, O_WRONLY | O_CREAT | O_APPEND, 0644,
						exit_or_return));
		if (res)
			return (1);
		redir = redir->next;
	}
	return (0);
}
