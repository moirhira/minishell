/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_command.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moirhira <moirhira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 20:27:40 by moirhira          #+#    #+#             */
/*   Updated: 2025/08/10 16:09:05 by moirhira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

typedef struct s_hd_data {
    char *delimiter_val;
    int k;
    int was_quote;
} t_hd_data;

static int process_quote(char *s, int i, t_hd_data *data, char quote)
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
    write(2, &quote, 1);
    ft_putstr_fd("\n", 2);
    return (-1);
}

static int heredoc_heleper(char *s, int i, t_hd_data *data)
{
	while (s[i] != '\0' && s[i] != ' ' && s[i] != '\t' 
        && s[i] != '|' && s[i] != '<' && s[i] != '>')
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
int handel_heredoc_delimiter(char *s, int i, t_token **token, int *state)
{
	t_hd_data data;
	int result;
	
    while (ft_isspace(s[i]))
		i++;
	data.delimiter_val = ft_calloc(ft_strlen(s) + 1, sizeof(char));
	if (!data.delimiter_val)
		return (-1);
	data.k = 0 ;
	data.was_quote = 0;
    result = heredoc_heleper(s, i, &data);
	if (result == -1)
		return (-1);
	add_token(token, create_token(data.delimiter_val, 0, 0, data.was_quote));
	*state = 0;
    return (result);
}

