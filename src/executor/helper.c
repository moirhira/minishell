/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekhallaf <ekhallaf@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 22:48:26 by ekhallaf          #+#    #+#             */
/*   Updated: 2025/07/15 20:05:50 by ekhallaf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../../include/minishell.h"

int is_valid_identifier(const char *str)
{
    int i = 1;
    if (!str || !str[0]) // check the first character of the arg 
        return 0;
    if (!isalpha(str[0]) && str[0] != '_')
        return 0;
    while(str[i]) // check the rest of the arg
    {
        if (!isalnum(str[i]) && str[i] != '_')
            return 0;
        i++;
    }
    return 1;
}

int is_alpha(char c)
{
    return ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'));
}

int is_digit(char c)
{
    return (c >= '0' && c <= '9');
}

int is_alnum(char c)
{
    return (is_alpha(c) || is_digit(c));
}

void    ft_swap(int *a, int *b)
{
    int temp;

    temp = *a;
    *a = *b;
    *b = temp;
}
