/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moirhira <moirhira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 22:07:56 by moirhira          #+#    #+#             */
/*   Updated: 2025/05/09 19:17:04 by moirhira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	free_token(t_token **token)
{
	t_token *current;
	t_token	*next;

	if (!token || !*token)
		return ;
	current = *token;
	while (current)
	{
		next = current->next;
		free(current);
		current = next;
	}
	*token = NULL;
}
void	free_command(t_command **command)
{
	
	t_command *current;
	t_command	*next;

	if (!command || !*command)
		return ;
	current = *command;
	if (!(*command)->next)
	{
		free(current);
		*command = NULL;
		return;
	}
	while (current)
	{
		next = current->next;
		free(current);
		current = next;
	}
	*command = NULL;
}

void	free_env(t_envp **env)
{
	
	t_envp *current;
	t_envp	*next;

	if (!env || !*env)
		return ;
	current = *env;
	if (!(*env)->next)
	{
		free(current);
		*env = NULL;
		return;
	}
	while (current)
	{
		next = current->next;
		free(current);
		current = next;
	}
	*env = NULL;
}