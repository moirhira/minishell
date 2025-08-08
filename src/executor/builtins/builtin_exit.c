/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moirhira <moirhira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 23:00:53 by ekhallaf          #+#    #+#             */
/*   Updated: 2025/08/08 15:54:37 by moirhira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

long my_strtol(const char *str, char **endptr)
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
        if (digit >= 10)
            break;
        result = result * 10 + digit;
        str++;
    }
    if (endptr != NULL) 
        *endptr = (char *)str;
    return result * sign;
}

int builtin_exit(char **args, t_envp **env)
{
    long code = 0;
    char *endptr;

    write(STDOUT_FILENO, "exit\n", 5);
    if (args[1])
    {
        code = my_strtol(args[1], &endptr);
        if (*endptr != '\0' || code < LONG_MIN || code > LONG_MAX)
        {
            display_error(ft_strjoin("exit: ", args[1]), "numeric argument required");
            fd_collector(-1, 1);
            free_all_memory();
            exit(exit_status(2));
        }
        if (args[2])
        {
            write(STDERR_FILENO, "exit: too many arguments\n", 25);
            exit_status(1);
            return 1;
        }
        exit_status((unsigned char)code);
        fd_collector(-1, 1);
        free_all_memory();
        exit((unsigned char)code);
    }
    fd_collector(-1, 1);
    free_all_memory();
    exit(exit_status(-1));
}




