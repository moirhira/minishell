/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moirhira <moirhira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 23:00:53 by ekhallaf          #+#    #+#             */
/*   Updated: 2025/08/07 17:28:02 by moirhira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

long my_strtol(const char *str, char **endptr)
{
    long result = 0;
    int sign = 1;
    
    while (*str == ' ' || *str == '\t' || *str == '\n') // Skip leading whitespace
        str++;
    if (*str == '-') // Handle + or -
    {
        sign = -1;
        str++;
    }
    else if (*str == '+')
        str++;
    while (*str >= '0' && *str <= '9') // Read digits
    {
        int digit = *str - '0';
        if (digit >= 10)
            break;
        result = result * 10 + digit;
        str++;
    }
    if (endptr != NULL) 
        *endptr = (char *)str; // Set endptr to where i stopped
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
            write(STDERR_FILENO, "exit: ", 6);
            write(STDERR_FILENO, args[1], ft_strlen(args[1]));
            write(STDERR_FILENO, ": numeric argument required\n", 28);
            free_all_memory();
            exit((unsigned char)code);  
            exit(2);
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




