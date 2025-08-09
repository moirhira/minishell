/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moirhira <moirhira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 23:00:53 by ekhallaf          #+#    #+#             */
/*   Updated: 2025/08/08 22:18:38 by moirhira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

static const char *skip_spaces_and_sign(const char *str, int *sign)
{
    while (*str == ' ' || *str == '\t' || *str == '\n')
        str++;
    if (*str == '-' || *str == '+')
    {
        if (*str == '-')
            *sign = -1;
        str++;
    }
    return str;
}

static int check_overflow(long result, int digit, int sign, int base)
{
    if (sign == 1 && result > (LONG_MAX - digit) / base)
        return 1;
    if (sign == -1 && result > ((unsigned long)-(LONG_MIN + digit)) / base)
        return -1;
    return 0;
}

long my_strtol(const char *str, char **endptr, int base)
{
    long result = 0;
    int sign = 1 ;
    int digit ; 
    int overflow;

    str = skip_spaces_and_sign(str, &sign);
    while (*str >= '0' && *str <= '9')
    {
        digit = *str - '0';
        if (digit >= base)
            break;
        overflow = check_overflow(result, digit, sign, base);
        if (overflow)
        {
            if (endptr)
                *endptr = (char *)str;
            if (overflow == 1)
                return LONG_MAX;
            else
                return LONG_MIN;
        }
        result = result * base + digit;
        str++;
    }
    if (endptr)
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
        code = my_strtol(args[1], &endptr, 10);
        if (*endptr != '\0')
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