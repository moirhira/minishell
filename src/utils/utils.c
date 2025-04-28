/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moirhira <moirhira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 22:07:56 by moirhira          #+#    #+#             */
/*   Updated: 2025/04/28 12:35:25 by moirhira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	free_token(t_token **stacka)
{
	t_token *current;
	t_token	*next;

	if (!stacka || !*stacka)
		return ;
	current = *stacka;
	while (current)
	{
		next = current->next;
		free(current);
		current = next;
	}
	*stacka = NULL;
}
void	free_command(t_command **command)
{
	t_command *current;
	t_command	*next;

	if (!command || !*command)
		return ;
	current = *command;
	while (current)
	{
		next = current->next;
		free(current);
		current = next;
	}
	*command = NULL;
}

char	*free_arr(char **res)
{
    int len;
    len = ft_strlen_2d(res);
	while (len > 0)
	{
		free(res[--len]);
	}
	free(res);
	return (NULL);
}