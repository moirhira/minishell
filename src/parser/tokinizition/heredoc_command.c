/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_command.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moirhira <moirhira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 20:27:40 by moirhira          #+#    #+#             */
/*   Updated: 2025/08/17 09:59:35 by moirhira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokinizition.h"

static int	process_quote(char *s, int i, t_herdoc_command *data, char quote)
{
	data->was_quote = 1;
	i++;
	while (s[i] && s[i] != quote)
	{
		data->delimiter_val[data->k++] = s[i++];
	}
	if (s[i] == quote)
		return (i + 1);
	ft_putstr_fd("minishell: Unclosed quote: ", 2);
	ft_putchar_fd(quote, 2);
	ft_putstr_fd("\n", 2);
	return (-1);
}

static int	heredoc_heleper(char *s, int i, t_herdoc_command *data)
{
	while (s[i] != '\0' && s[i] != ' ' && s[i] != '\t' && s[i] != '|'
		&& s[i] != '<' && s[i] != '>')
	{
		if (s[i] == '\'' || s[i] == '"')
		{
			i = process_quote(s, i, data, s[i]);
			if (i == -1)
				return (-1);
		}
		else
			data->delimiter_val[data->k++] = s[i++];
	}
	return (i);
}

static int	validate_err(char c, char c_p)
{
	ft_putstr_fd("minishell: syntax error near unexpected token `", 2);
	if (c_p != '\0' && c_p == c)
	{
		ft_putchar_fd(c, 2);
		ft_putchar_fd(c, 2);
	}
	else
		ft_putchar_fd(c, 2);
	ft_putstr_fd("'\n", 2);
	return (-1);
}

int	handel_heredoc_delimiter(char *s, int i, t_token **token, int *state)
{
	t_herdoc_command	data;
	int					result;

	while (ft_isspace(s[i]))
		i++;
	if (s[i] == '<' || s[i] == '>' || s[i] == '|')
	{
		return (validate_err(s[i], s[i + 1]));
	}
	data.delimiter_val = ft_calloc(ft_strlen(s) + 1, sizeof(char));
	if (!data.delimiter_val)
		return (-1);
	data.k = 0;
	data.was_quote = 0;
	result = heredoc_heleper(s, i, &data);
	if (result == -1)
		return (-1);
	add_token(token, create_token(data.delimiter_val, 0, 0, data.was_quote));
	*state = 0;
	return (result);
}
