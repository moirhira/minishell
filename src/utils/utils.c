/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moirhira <moirhira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 22:07:56 by moirhira          #+#    #+#             */
/*   Updated: 2025/05/14 21:19:27 by moirhira         ###   ########.fr       */
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
		free(current->value);
		free(current);
		current = next;
	}
	*token = NULL;
}
static void	free_redirects(t_redirect *redirect)
{
	t_redirect *next;

	while (redirect)
	{
		next = redirect->next;
		free(redirect->filename);
		free(redirect);
		redirect = next;
	}
}

void	free_command(t_command **command)
{
	int	i;
	t_command *current;
	t_command	*next;

	if (!command || !*command)
		return ;
	current = *command;
	if (!(*command)->next)
	{
		i = 0;
		if (current->args)
		{
			while(current->args[i])
				free(current->args[i++]);
			free(current->args);
		}
		// free(current->redirects->filename);
		if (current->redirects)
		{
			printf("redir\n");
			free_redirects(current->redirects);
		}
		free(current);
		*command = NULL;
		return;
	}
	while (current)
	{
		next = current->next;
		i = 0;
		if (current->args)
		{
			while(current->args[i])
				free(current->args[i++]);
			free(current->args);
		}
		if (current->redirects)
			free_redirects(current->redirects);
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
		free(current->key);
		free(current->value);
		free(current);
		current = next;
	}
	*env = NULL;
}