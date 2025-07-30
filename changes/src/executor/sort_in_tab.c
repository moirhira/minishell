/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort_in_tab.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moirhira <moirhira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 01:22:38 by ekhallaf          #+#    #+#             */
/*   Updated: 2025/07/24 20:55:18 by moirhira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"


void sort_in_tab(char **array, int size)
{
    int i = 0;
    while (i < size - 1)
    {
        int j = i + 1;
        while (j < size)
        {
            if (ft_strcmp(array[i], array[j]) > 0)
                ft_swap(&array[i], &array[j]);
            j++;
        }
        i++;
    }
}


