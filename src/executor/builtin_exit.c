/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekhallaf <ekhallaf@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 23:00:53 by ekhallaf          #+#    #+#             */
/*   Updated: 2025/07/19 13:37:54 by ekhallaf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

long my_strtol(const char *str, char **endptr, int base)
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
        if (digit >= base)
            break;
        result = result * base + digit;
        str++;
    }
    if (endptr != NULL) 
        *endptr = (char *)str; // Set endptr to where i stopped
    return result * sign;
}

// void free_env(t_envp **env)
// {
//     t_envp *tmp;

//     while (*env)
//     {
//         tmp = (*env)->next;
//         free((*env)->key);
//         free((*env)->value);
//         free(*env);
//         *env = tmp;
//     }
// }

int builtin_exit(char **args, t_envp **env)
{
    long code = 0;
    char *endptr;

    write(STDOUT_FILENO, "exit\n", 5);
    if (args[1])
    {
        code = my_strtol(args[1], &endptr, 10);
        //// If args[1] is not fully numeric OR overflows
        if (*endptr != '\0' || code < LONG_MIN || code > LONG_MAX)
        {
            write(STDERR_FILENO, "exit: ", 6);
            write(STDERR_FILENO, args[1], ft_strlen(args[1]));
            write(STDERR_FILENO, ": numeric argument required\n", 28);
            free_env(env);
            exit_status(255);
            exit(255);
        }
        //// If too many arguments (but first one is valid)
        if (args[2])
        {
            write(STDERR_FILENO, "exit: too many arguments\n", 25);
            exit_status(1);
            return 1; //// Do NOT exit
        }
        //// Valid numeric argument, cast to unsigned char
        exit_status((unsigned char)code);
        free_env(env);
        exit((unsigned char)code);
    }
    //// No argument: use last exit status
    free_env(env);
    exit(exit_status(0));
}




