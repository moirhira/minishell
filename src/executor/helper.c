/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moirhira <moirhira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 22:48:26 by ekhallaf          #+#    #+#             */
/*   Updated: 2025/08/05 19:05:14 by moirhira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../../include/minishell.h"

int is_valid_identifier(const char *str)
{
    int i = 1;
    if (!str || !str[0]) // check the first character of the arg 
        return 0;
    if (!ft_isalpha(str[0]) && str[0] != '_')
        return 0;
    while(str[i]) // check the rest of the arg
    {
        if (!ft_isalnum(str[i]) && str[i] != '_')
            return 0;
        i++;
    }
    return 1;
}

void    ft_swap(char **a, char **b)
{
    char *temp;

    temp = *a;
    *a = *b;
    *b = temp;
}
