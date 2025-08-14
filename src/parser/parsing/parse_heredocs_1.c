/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_heredocs_1.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moirhira <moirhira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 09:39:57 by moirhira          #+#    #+#             */
/*   Updated: 2025/08/14 11:18:49 by moirhira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

void	generate_unique_name(char temp_name[])
{
	unsigned int	random_num;
	int				fd;
	int				b_read;
	static int		counter;
	char			*random_str;

	ft_strcpy(temp_name, "/tmp/minishell_herdoc_");
	fd = open("/dev/urandom", O_RDONLY);
	if (fd != -1)
	{
		b_read = read(fd, &random_num, sizeof(random_num));
		close(fd);
		if (b_read != sizeof(random_num))
			random_num = counter;
	}
	else
		random_num = counter;
	counter++;
	random_str = ft_itoa((int)(random_num & 0xFFFFFF));
	ft_strcat(temp_name, random_str);
}

static int	process_herdoc(t_redirect *redir, t_envp *my_env)
{
	int		expand_var;
	char	*delimiter;
	char	temp_filename[256];

	delimiter = redir->filename;
	expand_var = (redir->type == TOKEN_HEREDOC);
	generate_unique_name(temp_filename);
	if (fill_herdoc(delimiter, expand_var, my_env, temp_filename))
	{
		redir->filename = ft_strdup(temp_filename);
		return (0);
	}
	else if (g_signal_received)
		return (1);
	return (0);
}

int	parse_heredocs(t_command *command, t_envp *my_env)
{
	t_redirect	*redir;
	t_command	*cmd;

	cmd = command;
	g_signal_received = 0;
	while (cmd)
	{
		if (cmd->heredoc_count > 0)
		{
			redir = cmd->redirects;
			while (redir)
			{
				if (redir->type == TOKEN_HEREDOC
					|| redir->type == TOKEN_HEREDOC_QUOTED)
				{
					if (process_herdoc(redir, my_env))
						return (1);
				}
				redir = redir->next;
			}
		}
		cmd = cmd->next;
	}
	return (0);
}
