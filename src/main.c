/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moirhira <moirhira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 21:08:03 by moirhira          #+#    #+#             */
/*   Updated: 2025/08/13 10:42:34 by moirhira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include "../libraries/libft/libft.h"

static void	reset_structs(t_token **t_list, t_command **c_list)
{
	*t_list = NULL;
	*c_list = NULL;
}

static char	*get_user_input(void)
{
	char	*cmd_line;

	cmd_line = read_input();
	if (!cmd_line)
	{
		free_all_memory();
		fd_collector(-1, 1);
		exit(exit_status(-1));
	}
	return (cmd_line);
}

static int	process_heredocs(t_command *list_cmd, t_envp *my_env)
{
	if (parse_heredocs(list_cmd, my_env) != 0)
	{
		if (g_signal_received)
		{
			exit_status(130);
			g_signal_received = 0;
		}
		return (1);
	}
	return (0);
}

void	shell_loop(t_envp *my_env, t_token **token_list, t_command **list_cmd)
{
	char	*cmd_line;

	while (1)
	{
		cmd_line = get_user_input();
		if (*cmd_line == '\0' || only_whitespace(cmd_line))
		{
			free(cmd_line);
			continue ;
		}
		if (parse_command(token_list, list_cmd, cmd_line, &my_env) != 0)
		{
			reset_structs(token_list, list_cmd);
			continue ;
		}
		free(cmd_line);
		if (process_heredocs(*list_cmd, my_env) != 0)
		{
			reset_structs(token_list, list_cmd);
			continue ;
		}
		execute_commands(*list_cmd, &my_env);
		reset_structs(token_list, list_cmd);
	}
}

int	main(int ac, char **av, char **env)
{
	char		cwd[PATH_MAX];
	t_token		*token_list;
	t_command	*list_cmd;
	t_envp		*my_env;

	(void)ac;
	(void)av;
	g_signal_received = 0;
	my_env = retrieve_envp(env);
	if (!my_env)
	{
		handle_export_arg(ft_strjoin("PWD=", getcwd(cwd, sizeof(cwd))),
			&my_env);
	}
	setup_signals(SHELL_INTERACTIVE);
	shell_loop(my_env, &token_list, &list_cmd);
	fd_collector(-1, 1);
	free_all_memory();
	return (exit_status(-1));
}
