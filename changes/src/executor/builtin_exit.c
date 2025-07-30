/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekhallaf <ekhallaf@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 23:00:53 by ekhallaf          #+#    #+#             */
/*   Updated: 2025/07/27 22:43:09 by ekhallaf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

long my_strtol(const char *str, char **endptr, int base)
{
    long result = 0;
    int sign = 1;
    
    while (*str == ' ' || *str == '\t' || *str == '\n')
        str++;
    if (*str == '-')
    {
        sign = -1;
        str++;
    }
    else if (*str == '+')
        str++;
    while (*str >= '0' && *str <= '9')
    {
        int digit = *str - '0';
        if (digit >= base)
            break;
        result = result * base + digit;
        str++;
    }
    if (endptr != NULL) 
        *endptr = (char *)str;
    return result * sign;
}

static void	exit_numeric_error(char *arg, t_envp **env)
{
	write(STDERR_FILENO, "exit: ", 6);
	write(STDERR_FILENO, arg, ft_strlen(arg));
	write(STDERR_FILENO, ": numeric argument required\n", 28);
	free_env(env);
	exit_status(2);
	exit(2);
}

static int	exit_too_many_args(void)
{
	write(STDERR_FILENO, "exit: too many arguments\n", 25);
	exit_status(1);
	return (1);
}

int	builtin_exit(char **args, t_envp **env)
{
	long	code;
	char	*endptr;

	write(STDOUT_FILENO, "exit\n", 5);
	if (!args[1])
	{
		free_env(env);
		exit(exit_status(0));
	}
	code = my_strtol(args[1], &endptr, 10);
	if (*endptr != '\0' || code < LONG_MIN || code > LONG_MAX)
		exit_numeric_error(args[1], env);
	if (args[2])
		return (exit_too_many_args());
	exit_status((unsigned char)code);
	free_env(env);
	exit((unsigned char)code);
}





