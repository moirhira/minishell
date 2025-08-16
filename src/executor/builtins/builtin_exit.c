/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moirhira <moirhira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 23:00:53 by ekhallaf          #+#    #+#             */
/*   Updated: 2025/08/16 08:54:33 by moirhira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

static void	exit_with_cleanup(int code)
{
	fd_collector(-1, 1);
	free_all_memory();
	exit(code);
}

int	builtin_exit(char **args)
{
	long	code;
	char	*endptr;

	code = 0;
	ft_putstr_fd("exit\n", 2);
	if (args[1])
	{
		code = my_strtol(args[1], &endptr, 10);
		if (*endptr != '\0')
		{
			display_error(ft_strjoin("exit: ", args[1]),
				"numeric argument required");
			exit_with_cleanup(exit_status(2));
		}
		if (args[2])
		{
			display_error("exit", "too many arguments");
			return (exit_status(1));
		}
		exit_status((unsigned char)code);
		exit_with_cleanup((unsigned char)code);
	}
	exit_with_cleanup(exit_status(-1));
	return (0);
}
