/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moirhira <moirhira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/19 15:51:44 by moirhira          #+#    #+#             */
/*   Updated: 2025/08/15 22:50:55 by moirhira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	**convert_env_to_array(t_envp *env)
{
	t_envp	*tmp;
	char	**envp;
	int		i;

	tmp = env;
	envp = ft_malloc(sizeof(char *) * (ft_lstsize(env) + 1));
	tmp = env;
	i = 0;
	while (tmp)
	{
		if (tmp->value == NULL)
			envp[i] = ft_strdup(tmp->key);
		else
		{
			envp[i] = ft_malloc(ft_strlen(tmp->key) + ft_strlen(tmp->value)
					+ 2);
			ft_strcpy(envp[i], tmp->key);
			ft_strcat(envp[i], "=");
			ft_strcat(envp[i], tmp->value);
		}
		tmp = tmp->next;
		i++;
	}
	envp[ft_lstsize(env)] = NULL;
	return (envp);
}

int	ft_lstsize(t_envp *lst)
{
	int	i;

	if (!lst)
		return (0);
	i = 0;
	while (lst != NULL)
	{
		lst = lst->next;
		i++;
	}
	return (i);
}

int	has_input_redir(t_command *cmd)
{
	t_redirect	*redir;

	if (!cmd || !cmd->redirects)
		return (0);
	redir = cmd->redirects;
	while (redir)
	{
		if (redir->type == TOKEN_INPUT || redir->type == TOKEN_HEREDOC
			|| redir->type == TOKEN_HEREDOC_QUOTED)
			return (1);
		redir = redir->next;
	}
	return (0);
}

int	has_output_redir(t_command *cmd)
{
	t_redirect	*redir;

	if (!cmd || !cmd->redirects)
		return (0);
	redir = cmd->redirects;
	while (redir)
	{
		if (redir->type == TOKEN_OUTPUT || redir->type == TOKEN_APPEND)
			return (1);
		redir = redir->next;
	}
	return (0);
}
