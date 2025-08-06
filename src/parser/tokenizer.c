/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moirhira <moirhira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 20:55:19 by moirhira          #+#    #+#             */
/*   Updated: 2025/08/06 22:21:56 by moirhira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char *handel_env_var(char *s, int *i, t_envp **my_env, char *curnt_str)
{
	if (!s[*i + 1] || ft_isspace(s[*i + 1]) || s[*i + 1] == '"')
	{
		char *temp = ft_strjoin(curnt_str, "$");
		// free(curnt_str);
		(*i)++;
		return (temp);
	}
	
	if (!ft_isalpha(s[*i + 1]) && s[*i + 1] != '_' && s[*i + 1] != '?' && s[*i + 1] != '"')
	{
		char *temp = ft_strjoin(curnt_str, "$");
		// free(curnt_str);
		(*i)++;
		return (temp);
	}
	
	(*i)++;

	if (s[*i] == '?')
	{
		(*i)++;
		char *exit_str = ft_itoa(exit_status(-1));
		char *temp = ft_strjoin(curnt_str, exit_str);
		// free(curnt_str);
		// free(exit_str);
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
		// free(curnt_str);
		curnt_str = temp;
	}
	// free(var_name);
	return (curnt_str);
}


char *get_var_value_and_advance(char *s, int *i, t_envp **my_env)
{
	// if (!s[*i + 1] || ft_isspace(s[*i + 1]) || s[*i + 1] == '"' || s[*i + 1] == '\'')
	if (!s[*i + 1] || ft_isspace(s[*i + 1]) || (s[*i + 1] == '"'))
	{
		(*i)++;
		return (ft_strdup("$"));
	}
	
	// Case: '$' followed by an invalid character (e.g., '$%'). Treat as a literal '$'.
	if (!ft_isalpha(s[*i + 1]) && s[*i + 1] != '_' && s[*i + 1] != '?' && s[*i + 1] != '"')
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
	// free(var_name);
	if (var_value)
	{
		return (ft_strdup(var_value));
	}
	return (NULL);
}



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

static int handel_simple_str(char *s, int i, t_envp **my_env, t_token **token)
{
	int attached;
	int dont_expand = 0;
	char *var_value;
	attached = was_previous_space(s, i);
	char *simple_str = ft_calloc(ft_strlen(s) * 2, 1);
	while (s[i] && s[i] != ' ' && s[i] != '\t' && s[i] != '\'' &&
		   s[i] != '"' && s[i] != '|' && s[i] != '>' && s[i] != '<')
	{
		if(s[i] == '=' && s[i + 1] && s[i + 1] == '$')
		{
			dont_expand = 1;
		}
		if (s[i] == '$')
		{
			if (dont_expand)
			{
				simple_str = handel_env_var(s, &i, my_env, simple_str);
			}
			else
			{
				var_value = get_var_value_and_advance(s, &i, my_env);
				if (!var_value || *var_value == '\0')
				{
					var_value = ft_strdup("");
					add_token(token, create_token(var_value, 0, 0, 0));
					get_last_token(*token)->ignored = 1;
					continue;
				}
				char **split_words = ft_split_advanced(var_value, " \t\n");
				if (!split_words || !split_words[0])
				{
					var_value = ft_strdup("");
					add_token(token, create_token(var_value, 0, 0, 0));
					get_last_token(*token)->ignored = 1;
					continue;
				}
				
				int word_count = ft_strlen_2d(split_words);
				char *temp = ft_strjoin(simple_str, split_words[0]);
				simple_str = temp;
				
				if (word_count > 1)
				{
					add_token(token, create_token(simple_str, 0, 0, 0));
					int k = 1;
					while (k < word_count - 1)
					{
						add_token(token, create_token(split_words[k], 0, 0, 0));
						k++;
					}
					simple_str = ft_strdup(split_words[word_count - 1]);
				}
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
		t_token *new = create_token(simple_str, 0, 0, 0);
		if (*token && attached)
			get_last_token(*token)->attached = 1;
		add_token(token, new);
	}
	return (i);
}

static int handel_quoted_str(char *s, int i, t_envp **my_env, t_token **token)
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
			
			// if(!(*token) || get_last_token((*token))->type != 5)
			// {
				final_str = handel_env_var(s, &i, my_env, final_str);
			// }
			// else
			// {
			// 	char ch[2] = {s[i++], '\0'};
			// 	char *temp = ft_strjoin(final_str, ch);
			// 	// free(final_str);
			// 	final_str = temp;
			// }
		}
		else
		{
			char ch[2] = {s[i++], '\0'};
			char *temp = ft_strjoin(final_str, ch);
			if (!temp)
			{
				// free(final_str);
				return (-1);
			}
			// free(final_str);
			final_str = temp;
		}
	}
	if (!s[i])
	{
		ft_putstr_fd("minishell: Unclosed quote:", 2);
		ft_putstr_fd(&quote, 2);
		ft_putstr_fd("\n", 2);
		// free(final_str);
		return (-1);
	}
	t_token *new = create_token(final_str, 0, 0, 1);
	if (*token && attached)
		get_last_token(*token)->attached = 1;
	add_token(token, new);
	// free(final_str);
	i++;
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
			{
				return (0);
			}
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
				i = handel_simple_str(s, i, my_env, token);
		}
	}
	return (1);
}
