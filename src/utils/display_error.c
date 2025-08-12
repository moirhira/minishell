/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display_error.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moirhira <moirhira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 15:48:00 by moirhira          #+#    #+#             */
/*   Updated: 2025/08/12 12:03:23 by moirhira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	display_error(char *command_or_file, char *error_message)
{
	char *full_error;
    int full_len;

    full_len = ft_strlen("minishell: ") + ft_strlen(command_or_file)
                 + ft_strlen(error_message) + 4;
    full_error = (char *)ft_malloc(full_len);
    if (!full_error)
        return;
        
    ft_strcpy(full_error ,"minishell: ");
    ft_strcat(full_error, command_or_file);
    ft_strcat(full_error, ": ");
    ft_strcat(full_error, error_message);
    ft_strcat(full_error, "\n");
    write(2, full_error, ft_strlen(full_error));
}