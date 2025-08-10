/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   qouted_command.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moirhira <moirhira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 13:42:05 by moirhira          #+#    #+#             */
/*   Updated: 2025/08/10 23:03:11 by moirhira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

typedef struct s_quote_data {
    char quote;
    char *final_str;
    t_envp **my_env;
} t_quote_data;

char *handel_varible(char *s, int *i, t_envp **my_env, char *curnt_str)
{
	if (s[*i] == '?')
	{
		(*i)++;
		char *exit_str = ft_itoa(exit_status(-1));
		char *temp = ft_strjoin(curnt_str, exit_str);
		return (temp);
	}

	int var_start = *i;
	while (s[*i] && (ft_isalnum(s[*i]) || s[*i] == '_'))
		(*i)++;
	char *var_name = ft_substr(s, var_start, *i - var_start);
	char *var_value = get_env_value(*my_env, var_name);
	if (var_value)
	{
		char *temp = ft_strjoin(curnt_str, var_value);
		curnt_str = temp;
	}
	return (curnt_str);
}

char *handel_env_var(char *s, int *i, t_envp **my_env, char *curnt_str)
{
	if (!s[*i + 1] || ft_isspace(s[*i + 1]) || s[*i + 1] == '"')
	{
		char *temp = ft_strjoin(curnt_str, "$");
		(*i)++;
		return (temp);
	}
	if (!ft_isalpha(s[*i + 1]) && s[*i + 1] != '_' 
		&& s[*i + 1] != '?' && s[*i + 1] != '"')
	{
		char *temp = ft_strjoin(curnt_str, "$");
		(*i)++;
		return (temp);
	}
	(*i)++;
	
	return (handel_varible(s, i, my_env, curnt_str));
}

static int quoted_helper(char *s, int *i, t_quote_data *data)
{
	char *temp;
	char ch[2];
	
	while (s[*i] && s[*i] != data->quote)
	{
		if (s[*i] == '$' && data->quote == '"')
			data->final_str = handel_env_var(s, i, data->my_env, data->final_str);
		else
		{
			ch[0] = s[(*i)++];
			ch[1] =  '\0';
			temp = ft_strjoin(data->final_str, ch);
			data->final_str = temp;
		}
	}
	if (!s[*i])
	{
		ft_putstr_fd("minishell: Unclosed quote: ", 2);
		ft_putchar_fd(data->quote, 2);
		ft_putstr_fd("\n", 2);
		return (0);
	}
	return (1);
}


int handel_quoted_str(char *s, int i, t_envp **my_env, t_token **token)
{
	int attached;
	t_quote_data data;
	
	
	attached = was_previous_space(s, i);
	data.quote = s[i++];
	data.final_str = ft_calloc(ft_strlen(s), 1);
	data.my_env = my_env;
	if (!quoted_helper(s, &i, &data))
		return (-1);
	if (*token && attached)
		get_last_token(*token)->attached = 1;
	add_token(token, create_token(data.final_str, 0, 0, 1));
	i++;
	return (i);
}

