/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moirhira <moirhira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 20:55:19 by moirhira          #+#    #+#             */
/*   Updated: 2025/05/03 20:37:26 by moirhira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static char *handel_env_var(char *s, int *i, char **my_env, char *curnt_str)
{
	(*i)++;
	if (s[*i] == '?')
	{
		(*i)++;
		char *exit_str = ft_itoa(g_last_exit_status);
		char *temp = ft_strjoin(curnt_str, exit_str);
		free(curnt_str);
		free(exit_str);
		return (temp);
	}
	int var_start = *i;
	while (s[*i]  && (ft_isalnum(s[*i]) || s[*i] == '_'))
		(*i)++;
	char *var_name = ft_substr(s, var_start, *i - var_start);
	char *var_value = get_env_value(my_env, var_name);
	if (var_value)
	{
		char *temp = ft_strjoin(curnt_str, var_value);
		free(curnt_str);
		curnt_str = temp;
	}
	free(var_name);
	return (curnt_str);
}

static int handel_operator(char *s, int i, t_token **token)
{
	char *symb;
	
	if ((s[i] == '>' && s[i + 1] == '>') || (s[i] == '<' && s[i + 1] == '<'))
	{
		char symb_alloc[3] = {s[i], s[i], '\0'};
		symb = ft_strdup(symb_alloc);
		if(ft_strcmp(symb, ">>") == 0)
			add_token(token, create_token(symb, 4, 0));
		else if(ft_strcmp(symb, "<<") == 0)
			add_token(token, create_token(symb, 5, 0));
		i += 2;
	}
	else
	{
		char symb_alloc[2] = {s[i], '\0'};
		symb = ft_strdup(symb_alloc);
		if(ft_strcmp(symb, "|") == 0)
			add_token(token, create_token(symb, 1, 0));
		else if(ft_strcmp(symb, "<") == 0)
			add_token(token, create_token(symb, 2, 0));
		else if(ft_strcmp(symb, ">") == 0)
			add_token(token, create_token(symb, 3, 0));
		i++;
	}
	return (i);
}

static int handel_simple_str(char *s, int i, char **my_env, t_token **token)
{
	int attached;
	attached = was_previous_space(s, i);
	char *simple_str = ft_calloc(ft_strlen(s) * 2 + 1, 1);
	while (s[i] && s[i] != ' ' && s[i] != '\t'  && s[i] != '\'' &&
		s[i] != '"' && s[i] != '|' && s[i] != '>' && s[i] != '<' )
	{
		if(s[i] == '$')
			simple_str = handel_env_var(s, &i, my_env, simple_str);
		else
		{
			char ch[2] = {s[i++], '\0'};
			char *temp = ft_strjoin(simple_str, ch);
			free(simple_str);
			simple_str = temp;
		}
	}
	t_token *new = create_token(simple_str, 0, 0);
	if (*token && attached)
		get_last_token(*token)->attached = 1;
	add_token(token,new);
	return (i);
}

static int handel_quoted_str(char *s, int i, char **my_env, t_token **token)
{
	int attached;
	attached = was_previous_space(s, i);
	char quote = s[i++];
	char *final_str = ft_calloc(ft_strlen(s) * 2 + 1, 1);
	while (s[i] && s[i] != quote)
	{
		if (s[i] == '$' && quote == '"')
			final_str = handel_env_var(s, &i, my_env, final_str);
		else 
		{
			char ch[2] = {s[i++], '\0'};
			char *temp = ft_strjoin(final_str, ch);
			free(final_str);
			final_str = temp;
		}
	}
	if (!s[i])
	{
		printf("minishell: Unclose quote: %c\n",quote);
		free(final_str);
		return (-1);
	}
	t_token *new = create_token(final_str, 0, 0);
	if (*token && attached)
		get_last_token(*token)->attached = 1;
	add_token(token,new);
	i++;
	return(i);
}

t_token	*split_token(char *s, char **my_env, t_token **token)
{
	int i = 0;
	while (s[i])
	{
		while (s[i] == ' ' || s[i] == '\t')
			i++;
		if (!s[i])
			break;
		if (s[i] == '\'' || s[i] == '"')
		{
			i = handel_quoted_str(s, i, my_env, token);
			if (i == -1)
			{	
				free_arr(my_env);
				free_token(token);
				exit(EXIT_FAILURE);
			}
		}
		else if (s[i] == '|' || s[i] == '>' || s[i] == '<')
			i = handel_operator(s, i, token);
		else
			i = handel_simple_str(s,i,my_env,token);
	}
	return (t_token *)(1);
}
