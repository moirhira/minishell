/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moirhira <moirhira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 09:15:44 by moirhira          #+#    #+#             */
/*   Updated: 2025/08/13 10:09:02 by moirhira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	only_whitespace(char *str)
{
	int	i;

	i = 0;
	while (str[i] != '\0')
	{
		if (!ft_isspace(str[i]))
			return (0);
		i++;
	}
	return (1);
}

char	*read_input(void)
{
	char	*line;

	line = readline("minishell$ ");
	if (!line)
		return (NULL);
	if (*line)
		add_history(line);
	return (line);
}

t_envp	*retrieve_envp(char **env)
{
	int		i;
	t_envp	*head;
	char	*equal_sign;
	size_t	key_len;
	char	*key;

	head = NULL;
	i = 0;
	while (env[i] != NULL)
	{
		equal_sign = ft_strchr(env[i], '=');
		if (equal_sign)
		{
			key_len = equal_sign - env[i];
			key = ft_strndup(env[i], key_len);
			append_env(&head, new_env_node(key, equal_sign + 1));
		}
		i++;
	}
	return (head);
}
