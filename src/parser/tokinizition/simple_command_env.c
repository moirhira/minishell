/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simple_command_env.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moirhira <moirhira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 20:27:20 by moirhira          #+#    #+#             */
/*   Updated: 2025/08/10 21:18:01 by moirhira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

static char	*handel_single_quote(char *s, int *i, int *q)
{
	int		start;
	char	*string;

	*i += 2;
	start = *i;
	while (s[*i] && s[*i] != '\'')
		(*i)++;
	if (!s[*i])
	{
		*q = 1;
		ft_putstr_fd("minishell: Unclosed quote: '\n", 2);
		return (NULL);
	}
	string = ft_substr(s, start, *i - start);
	if (s[*i] == '\'')
		(*i)++;
	return (string);
}

static char	*handel_double_quote_conten(char *s, int *i, t_envp **my_env,
		int *q)
{
	char	*res;
	char	*var_val;
	char	*temp;
	char	*str;

	res = ft_strdup("");
	while (s[*i] && s[*i] != '"')
	{
		if (s[*i] == '$')
		{
			var_val = get_var_value_and_advance(s, i, my_env, q);
			if (!var_val)
				var_val = ft_strdup("");
			temp = ft_strjoin(res, var_val);
			res = temp;
		}
		else
		{
			str = ft_substr(s, *i, 1);
			temp = ft_strjoin(res, str);
			res = temp;
			(*i)++;
		}
	}
	return (res);
}

static char	*handel_double_quote(char *s, int *i, t_envp **my_env, int *q)
{
	char	*res;

	*i += 2;
	res = handel_double_quote_conten(s, i, my_env, q);
	if (!s[*i])
	{
		*q = 1;
		ft_putstr_fd("minishell: Unclosed quote: \"\n", 2);
		return (NULL);
	}
	if (s[*i] == '"')
		(*i)++;
	return (res);
}

static char	*handel_variable(char *s, int *i, t_envp **my_env)
{
	int		var_start;
	char	*var_name;
	char	*var_value;

	if (s[*i] == '?')
	{
		(*i)++;
		return (ft_itoa(exit_status(-1)));
	}
	var_start = *i;
	while (s[*i] && (ft_isalnum(s[*i]) || s[*i] == '_'))
		(*i)++;
	var_name = ft_substr(s, var_start, *i - var_start);
	var_value = get_env_value(*my_env, var_name);
	if (var_value)
		return (ft_strdup(var_value));
	return (NULL);
}

char	*get_var_value_and_advance(char *s, int *i, t_envp **my_env, int *q)
{
	if (s[*i + 1] && (s[*i + 1] == '\''))
		return (handel_single_quote(s, i, q));
	if (s[*i + 1] == '"')
		return (handel_double_quote(s, i, my_env, q));
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
	return (handel_variable(s, i, my_env));
}
