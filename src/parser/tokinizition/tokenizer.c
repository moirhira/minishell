/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moirhira <moirhira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 20:55:19 by moirhira          #+#    #+#             */
/*   Updated: 2025/08/09 20:59:37 by moirhira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"


static int handel_operator(char *s, int i, t_token **token, int *state)
{
	char *symb;

	if ((s[i] == '>' && s[i + 1] == '>') || (s[i] == '<' && s[i + 1] == '<'))
	{
		char symb_alloc[3] = {s[i], s[i], '\0'};
		symb = ft_strdup(symb_alloc);
		if (ft_strcmp(symb, ">>") == 0)
			add_token(token, create_token(symb, 4, 0, 0));
		else if (ft_strcmp(symb, "<<") == 0)
		{
			add_token(token, create_token(symb, 5, 0, 0));
			*state = 1;
		}
		i += 2;
	}
	else
	{
		char symb_alloc[2] = {s[i], '\0'};
		symb = ft_strdup(symb_alloc);
		if (ft_strcmp(symb, "|") == 0)
			add_token(token, create_token(symb, 1, 0, 0));
		else if (ft_strcmp(symb, "<") == 0)
			add_token(token, create_token(symb, 2, 0, 0));
		else if (ft_strcmp(symb, ">") == 0)
			add_token(token, create_token(symb, 3, 0, 0));
		i++;
	}
	return (i);
}


int split_token(char *s, t_envp **my_env, t_token **token)
{
	int i = 0;
	int state;

	state = 0;
	while (s[i])
	{
		while (s[i] == ' ' || s[i] == '\t')
			i++;
		if (!s[i])
			break;
		if (state == 1)
		{
			i = handel_heredoc_delimiter(s, i, token, &state);
			if (i == -1)
				return (0);
		}
		else
		{
			if (s[i] == '\'' || s[i] == '"')
			{
				i = handel_quoted_str(s, i, my_env, token);
				if (i == -1)
				{
					return (0);
				}
			}
			else if (s[i] == '|' || s[i] == '>' || s[i] == '<')
				i = handel_operator(s, i, token, &state);
			else
			{
				i = handel_simple_str(s, i, my_env, token);
				if (i == -1)
					return (0);
			}
		}
	}
	return (1);
}
