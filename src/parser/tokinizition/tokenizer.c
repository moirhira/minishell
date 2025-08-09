/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moirhira <moirhira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 20:55:19 by moirhira          #+#    #+#             */
/*   Updated: 2025/08/09 13:41:34 by moirhira         ###   ########.fr       */
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

int handel_heredoc_delimiter(char *s, int i, t_token **token, int *state)
{
	int 	k;
	int was_qoute;
	char    *delimiter_val;
	
    while (ft_isspace(s[i]))
		i++;
	delimiter_val = ft_calloc(ft_strlen(s) + 1, sizeof(char));
	if (!delimiter_val)
		return (-1);
	k = 0 ;
	was_qoute = 0;
    while (s[i] != '\0' && s[i] != ' ' && s[i] != '\t' && s[i] != '|' && s[i] != '<' && s[i] != '>')
    {
        if (s[i] == '\'' || s[i] == '"')
        {
			char quote = s[i];
			was_qoute = 1;
			i++;
			while (s[i] && s[i] != quote)
			{
				delimiter_val[k++] = s[i++];
			}
			if (s[i] == quote)
			{
				i++;
			}
			else
			{
				ft_putstr_fd("minishell: Unclosed quote:", 2);
				ft_putstr_fd(&quote, 2);
				ft_putstr_fd("\n", 2);
				return (-1);
			}
        }
        else
        {
            delimiter_val[k++] = s[i++];
        }
    }
	add_token(token, create_token(delimiter_val, 0, 0, was_qoute));
	*state = 0;
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
