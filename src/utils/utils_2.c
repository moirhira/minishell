/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moirhira <moirhira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 21:50:32 by moirhira          #+#    #+#             */
/*   Updated: 2025/07/18 21:51:18 by moirhira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int exit_status(int new_status)
{
	static int status = 0;

	if (new_status != -1)
		status = new_status;
	return (status);
}

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