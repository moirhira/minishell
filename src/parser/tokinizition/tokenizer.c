/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moirhira <moirhira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 20:55:19 by moirhira          #+#    #+#             */
/*   Updated: 2025/08/10 15:33:56 by moirhira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

typedef struct s_split_data
{
    t_envp **my_env;
    t_token **token;
    int state;
} t_split_data;


int process_token(char *s, int i, t_split_data *data)
{
	if (data->state == 1)
	{
		i = handel_heredoc_delimiter(s, i, data->token, &data->state);
		if (i == -1)
			return (-1);
	}
	else
	{
		if (s[i] == '\'' || s[i] == '"')
		{
			i = handel_quoted_str(s, i, data->my_env, data->token);
			if (i == -1)
				return (-1);
		}
		else if (s[i] == '|' || s[i] == '>' || s[i] == '<')
			i = handel_operator(s, i, data->token, &data->state);
		else
		{
			i = handel_simple_str(s, i, data->my_env, data->token);
			if (i == -1)
				return (-1);
		}
	}
	return (i);
}


int split_token(char *s, t_envp **my_env, t_token **token)
{
	int i;
	t_split_data data;
	int result;
	
	i = 0;
	data.my_env = my_env;
	data.token = token;
	data.state = 0;
	while (s[i])
	{
		while (s[i] == ' ' || s[i] == '\t')
			i++;
		if (!s[i])
			break;
		result = process_token(s, i, &data);
		if (result == -1)
			return (0);
		i = result;
	}
	return (1);
}
