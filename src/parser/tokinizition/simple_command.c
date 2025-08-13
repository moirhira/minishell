/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simple_command.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moirhira <moirhira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 13:40:58 by moirhira          #+#    #+#             */
/*   Updated: 2025/08/13 11:23:46 by moirhira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokinizition.h"

static void	process_dolar(char *s, int *i, t_simple_command *data)
{
	char	*var_value;
	char	*temp;

	if (data->dont_trim)
	{
		data->simple_str = handel_env_var(s, i, data->my_env, data->simple_str);
	}
	else
	{
		var_value = get_var_value_and_advance(s, i, data->my_env,
				&data->unclose_quote);
		if (!var_value)
		{
			add_token(data->token, create_token(ft_strdup(""), 0, 0, 0));
			get_last_token(*(data->token))->ignored = 1;
			return ;
		}
		temp = ft_strjoin(data->simple_str, var_value);
		data->simple_str = temp;
	}
}

static void	process_regular_char(char *s, int *i, t_simple_command *data)
{
	char	ch[2];
	char	*temp;

	ch[0] = s[(*i)++];
	ch[1] = '\0';
	temp = ft_strjoin(data->simple_str, ch);
	data->simple_str = temp;
}

static int	extract_simple_str(char *s, int i, t_simple_command *data)
{
	while (s[i] && s[i] != ' ' && s[i] != '\t' && s[i] != '\'' && s[i] != '"'
		&& s[i] != '|' && s[i] != '>' && s[i] != '<')
	{
		if (s[i] == '=' && s[i + 1] && s[i + 1] == '$')
			data->dont_trim = 1;
		if (s[i] == '$')
		{
			process_dolar(s, &i, data);
		}
		else
		{
			process_regular_char(s, &i, data);
		}
		if (data->unclose_quote)
			return (-1);
	}
	return (i);
}

static void	process_split_words(char **split_words, t_simple_command *data)
{
	int		k;
	t_token	*new;

	k = 0;
	while (split_words[k] != NULL)
	{
		new = create_token(ft_strdup(split_words[k]), 0, 0, 0);
		if (*(data->token) && data->attached)
			get_last_token(*(data->token))->attached = 1;
		add_token(data->token, new);
		k++;
	}
}

int	handel_simple_str(char *s, int i, t_envp **my_env, t_token **token)
{
	t_simple_command	data;
	char				**split_words;

	data.attached = was_previous_space(s, i);
	data.dont_trim = 0;
	data.unclose_quote = 0;
	data.simple_str = ft_calloc(1, 1);
	data.my_env = my_env;
	data.token = token;
	i = extract_simple_str(s, i, &data);
	if (i == -1)
		return (-1);
	if (data.simple_str && *(data.simple_str))
	{
		split_words = ft_split_advanced(data.simple_str, " \t\n");
		if (!split_words || !split_words[0])
		{
			add_token(token, create_token(ft_strdup(""), 0, 0, 0));
			get_last_token(*token)->ignored = 1;
		}
		else
			process_split_words(split_words, &data);
	}
	return (i);
}
