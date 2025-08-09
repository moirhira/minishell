/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simple_command.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moirhira <moirhira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 13:40:58 by moirhira          #+#    #+#             */
/*   Updated: 2025/08/09 13:42:39 by moirhira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

char *get_var_value_and_advance(char *s, int *i, t_envp **my_env, int *q)
{
	
	if (s[*i + 1] && (s[*i + 1] == '\''))
	{
		*i += 2;
		int start = *i;
		while (s[*i] && s[*i] != '\'')
			(*i)++;
		if (!s[*i])
		{
			*q = 1;
			ft_putstr_fd("minishell: Unclosed quote: '\n", 2);
			return (NULL);
		}
		char *string = ft_substr(s, start, *i - start);
		if (s[*i] == '\'')
            (*i)++;
		return (string);
	}
	
	if (s[*i + 1] == '"')
    {
        *i += 2; 
        char *res = ft_strdup("");
        while (s[*i] && s[*i] != '"')
        {
            if (s[*i] == '$')
			{
				char *var_val = get_var_value_and_advance(s, i, my_env, q);
				if (!var_val)
					var_val = ft_strdup("");
				char *temp = ft_strjoin(res, var_val);
            	res = temp;
			} 
            else
			{
				char *str = ft_substr(s, *i , 1);
				char *tmp = ft_strjoin(res, str);
				res = tmp;
				(*i)++;
			}
        }
		if (!s[*i])
		{
			*q = 1;
			ft_putstr_fd("minishell: Unclosed quote: \"\n", 2);
			return (NULL);
		}
        if (s[*i] == '"')
            (*i)++;
        return res;
    }
	
	if (!s[*i + 1] || ft_isspace(s[*i + 1]))
	{
		(*i)++;
		return (ft_strdup("$"));
	}
	if (!ft_isalpha(s[*i + 1]) && s[*i + 1] != '_' && s[*i + 1] != '?')
	{
		(*i)++;
		return (ft_strdup("$"));
	}
	
	(*i)++;

	if (s[*i] == '?')
	{
		(*i)++;
		return (ft_itoa(exit_status(-1)));
	}

	int var_start = *i;
	while (s[*i] && (ft_isalnum(s[*i]) || s[*i] == '_'))
		(*i)++;
	char *var_name = ft_substr(s, var_start, *i - var_start);
	char *var_value = get_env_value(*my_env, var_name);
	if (var_value)
		return (ft_strdup(var_value));
	return (NULL);
}

int handel_simple_str(char *s, int i, t_envp **my_env, t_token **token)
{
    int attached = was_previous_space(s, i);
    char *var_value;
	int dont_trim = 0;
    char *simple_str = ft_calloc(1, 1);
	int unclose_qoute = 0;
    while (s[i] && s[i] != ' ' && s[i] != '\t' && s[i] != '\'' &&
           s[i] != '"' && s[i] != '|' && s[i] != '>' && s[i] != '<')
    {
		if(s[i] == '=' && s[i + 1] && s[i + 1] == '$')
		{
			dont_trim = 1;
		}
        if (s[i] == '$')
        {
			if (dont_trim)
			{
				simple_str = handel_env_var(s, &i, my_env, simple_str);
			}
            else
			{
				var_value = get_var_value_and_advance(s, &i, my_env, &unclose_qoute);
				if (unclose_qoute)
					return (-1);
				if (!var_value)
				{
					add_token(token, create_token(ft_strdup(""), 0, 0, 0));
					get_last_token(*token)->ignored = 1;
					continue;
				}

				char *temp = ft_strjoin(simple_str, var_value);
				simple_str = temp;
			}
        }
        else
        {
            char ch[2] = {s[i++], '\0'};
            char *temp = ft_strjoin(simple_str, ch);
            simple_str = temp;
        }
    }
	
    if (simple_str && *simple_str)
    {
        char **split_words = ft_split_advanced(simple_str, " \t\n");

        if (!split_words || !split_words[0])
        {
            add_token(token, create_token(ft_strdup(""), 0, 0, 0));
            get_last_token(*token)->ignored = 1;
        }
        else
        {
			int k = 0;
            while (split_words[k] != NULL)
            {
                t_token *new = create_token(ft_strdup(split_words[k]), 0, 0, 0);
                if (*token && attached)
					get_last_token(*token)->attached = 1;
                
                add_token(token, new);
				k++;
            }
        }
    }
    return (i);
}