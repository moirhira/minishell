/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit_helper.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moirhira <moirhira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 20:40:22 by moirhira          #+#    #+#             */
/*   Updated: 2025/08/12 22:22:59 by moirhira         ###   ########.fr       */
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
    return (str);
}

static int check_overflow(unsigned long result, int digit, int sign, int base)
{
    if (sign == 1 && result > (unsigned long)(LONG_MAX - digit) / base)
        return 1;
    if (sign == -1 && result > ((unsigned long)-(LONG_MIN + digit)) / base)
        return -1;
    return 0;
}

static long handle_overflow(int overflow, const char *str, char **endptr)
{
    if (endptr)
        *endptr = (char *)str;
    if (overflow == 1)
        return LONG_MAX;
    else
        return LONG_MIN;
}
long my_strtol(const char *str, char **endptr, int base)
{
    unsigned long result;
    int sign;
    int digit ; 
    int overflow;
    
    result = 0;
    sign = 1;
    str = skip_spaces_and_sign(str, &sign);
    while (*str >= '0' && *str <= '9')
    {
        digit = *str - '0';
        if (digit >= base)
            break;
        overflow = check_overflow(result, digit, sign, base);
        if (overflow)
            return (handle_overflow(overflow, str, endptr));
        result = result * base + digit;
        str++;
    }
    while (ft_isspace(*str))
        str++;
    if (endptr)
        *endptr = (char *)str;
    return (result * sign);
}

