/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   qouted_command.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moirhira <moirhira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 13:42:05 by moirhira          #+#    #+#             */
/*   Updated: 2025/08/09 13:43:39 by moirhira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

char *handel_env_var(char *s, int *i, t_envp **my_env, char *curnt_str)
{
	if (!s[*i + 1] || ft_isspace(s[*i + 1]) || s[*i + 1] == '"')
	{
		char *temp = ft_strjoin(curnt_str, "$");
		(*i)++;
		return (temp);
	}
	
	if (!ft_isalpha(s[*i + 1]) && s[*i + 1] != '_' && s[*i + 1] != '?' && s[*i + 1] != '"')
	{
		char *temp = ft_strjoin(curnt_str, "$");
		(*i)++;
		return (temp);
	}
	
	(*i)++;

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


int handel_quoted_str(char *s, int i, t_envp **my_env, t_token **token)
{
	int attached;
	attached = was_previous_space(s, i);
	char quote = s[i++];
	char *final_str = ft_calloc(ft_strlen(s), 1);
	if (!final_str)
		return (-1);
	while (s[i] && s[i] != quote)
	{
		if (s[i] == '$' && quote == '"')
		{
			final_str = handel_env_var(s, &i, my_env, final_str);
		}
		else
		{
			char ch[2] = {s[i++], '\0'};
			char *temp = ft_strjoin(final_str, ch);
			if (!temp)
				return (-1);
			final_str = temp;
		}
	}
	if (!s[i])
	{
		ft_putstr_fd("minishell: Unclosed quote:", 2);
		ft_putstr_fd(&quote, 2);
		ft_putstr_fd("\n", 2);
		return (-1);
	}
	t_token *new = create_token(final_str, 0, 0, 1);
	if (*token && attached)
		get_last_token(*token)->attached = 1;
	add_token(token, new);
	i++;
	return (i);
}