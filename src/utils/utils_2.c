/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moirhira <moirhira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/19 15:51:44 by moirhira          #+#    #+#             */
/*   Updated: 2025/07/19 15:51:58 by moirhira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../../include/minishell.h"

int only_whitespace(char *str)
{
    int i;

    i = 0;
    while (str[i] != '\0')
    {
        if(!ft_isspace(str[i]))
            return (0);
        i++;
    }
    return (1);
}