/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekhallaf <ekhallaf@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 22:48:26 by ekhallaf          #+#    #+#             */
/*   Updated: 2025/07/27 20:16:14 by ekhallaf         ###   ########.fr       */
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
