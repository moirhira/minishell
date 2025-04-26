/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_token.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moirhira <moirhira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 20:55:19 by moirhira          #+#    #+#             */
/*   Updated: 2025/04/26 21:08:14 by moirhira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdio.h>

char *get_env_value(char **my_env, const char *var_name)
{
	int i = 0;
	size_t len = ft_strlen(var_name);
	
	while (my_env[i])
	{
		if (ft_strncmp(my_env[i], var_name, len) == 0 && my_env[i][len] == '=')
			return(my_env[i] + len + 1);
		i++;

	}
	return (NULL);
	
}
static char	**free_split(char **res, size_t indx)
{
	while (indx > 0)
	{
		free(res[--indx]);
	}
	free(res);
	return (NULL);
}
static char	*ft_memalloc(char const *s, int start, int end)
{
	int		i;
	char		*ptr;
	int len;

	len = end - start;
	ptr = (char *)malloc(sizeof(char) * (len + 1));
	if (!ptr)
		return (NULL);
	i = 0;
	while (start < end)
	{
		ptr[i] = s[start];
		i++;
		start++;
	}
	ptr[len] = '\0';
	return (ptr);
}
int split_symbols(char **res, char *str, int k, char symb)
{
	int x = 0;
	int len = ft_strlen(str);				
	while (x < len)
	{
		if ((str[x] == '>' && str[x + 1] == '>') || (str[x] == '<' && str[x + 1] == '<'))
		{
			char symb_alloc[3] ;
			symb_alloc[0] = str[x];
			symb_alloc[1] = str[x];
			symb_alloc[2] = '\0';
			res[k++] = ft_strdup(symb_alloc);
			x += 2;
			// break;
		}
		else if (str[x] == symb)
		{
			char symb_alloc[2] ;
			symb_alloc[0] = symb;
			symb_alloc[1] = '\0';
			res[k++] = ft_strdup(symb_alloc);
			x++;
		}
		else
		{
			int s_start = x;
			while (str[x] && str[x] != symb && str[x] != ' ' && str[x] != '\t')
				x++;
			res[k++] = ft_memalloc(str, s_start,x);
		}
	}
	return (k);
}
char	**split_token(char *s, char **my_env)
{
	char	**res = (char **)malloc(sizeof(char *) * 100);
	if (!res)
		return (NULL);
	int i = 0;
	int k = 0;
	while (s[i])
	{
		while (s[i] == ' ' || s[i] == '\t')
			i++;
		if (!s[i])
			break;
		if (s[i] == '\'' || s[i] == '"')
		{
			char quote = s[i++];
			char *final_str = ft_calloc(ft_strlen(s) * 2 + 1, 1);
			while (s[i] && s[i] != quote)
			{
				if (s[i] == '$' && quote == '"')
				{
					i++;
					int var_start = i;
					while (s[i]  && (ft_isalnum(s[i]) || s[i] == '_'))
						i++;
					char *var_name = ft_substr(s, var_start, i - var_start);
					char *var_value = get_env_value(my_env, var_name);
					if (var_value)
					{
						char *temp = ft_strjoin(final_str, var_value);
						free(final_str);
						final_str = temp;
						
					}
					free(var_name);
				}
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
				printf("msh: Unclose quote: %c\n",quote);
				free_split(res, k);
				return (NULL);
			}
            res[k++] = final_str;
			i++;
		}
		else if (s[i] == '|' || s[i] == '>' || s[i] == '<')
		{	
			if ((s[i] == '>' && s[i + 1] == '>') || (s[i] == '<' && s[i + 1] == '<'))
			{
				char symb_alloc[3] ;
				symb_alloc[0] = s[i];
				symb_alloc[1] = s[i];
				symb_alloc[2] = '\0';
				res[k++] = ft_strdup(symb_alloc);
				i += 2;
			}
			else
			{
				char symb_alloc[2] ;
				symb_alloc[0] = s[i];
				symb_alloc[1] = '\0';
				res[k++] = ft_strdup(symb_alloc);
				i++;
			}
		}
		else
		{
			char *simple_str = ft_calloc(ft_strlen(s) * 2 + 1, 1);
			int j = 0;
			while (s[i] && s[i] != ' ' && s[i] != '\t'  && s[i] != '\'' &&
				 s[i] != '"' && s[i] != '|' && s[i] != '>' && s[i] != '<' )
			{
				if(s[i] == '$')
				{
					i++;
					int var_start = i;
					while(s[i] && (ft_isalnum(s[i]) || s[i] == '_'))
						i++;
					char *var_name = ft_substr(s, var_start, i - var_start);
					char *var_value = get_env_value(my_env, var_name);
					if (var_value)
					{
						char *temp = ft_strjoin(simple_str,var_value);
						free(simple_str);
						simple_str = temp;
					}
					free(var_name);
				}
				else
				{
					char ch[2] = {s[i++], '\0'};
					char *temp = ft_strjoin(simple_str, ch);
					free(simple_str);
					simple_str = temp;
				}
			}
			res[k++] = simple_str;
		}
	}
	res[k] = NULL;
	return (res);
}


//check the env variabless

// }
		// else
		// {
		// 	start = i;
		// 	while (s[i] && s[i] != ' ' && s[i] != '\t')
		// 		i++;
		// 	end = i;
		// 	while (s[i] == ' ' || s[i] == '\t')
		// 		i++;
		// 	res[k] = ft_memalloc(s, start, end);
		// 	if(!res[k])
		// 		exit(1);
			// char *crnt_str = ft_strdup(res[k]);
			// if(ft_strchr(crnt_str, '|')) 
			// 	k = split_symbols(res, crnt_str, k, '|');
			// else if(ft_strchr(crnt_str, '>')) 
			// 	k = split_symbols(res, crnt_str, k, '>');
			// else if(ft_strchr(crnt_str, '<')) 
			// 	k = split_symbols(res, crnt_str, k, '<');
			// else if (ft_strchr(crnt_str, '"') || ft_strchr(crnt_str, '\''))
			// {
			// 	int i = 0;
			// 	int j = 0;
			// 	char *final_str = malloc(ft_strlen(crnt_str));
			// 	while (crnt_str[i] != '\'' && crnt_str[i] != '"')
			// 	{
			// 		final_str[j] = crnt_str[i];
			// 		j++;
			// 		i++;
			// 	}
			// 	while (crnt_str[i] == '\'' || crnt_str[i] == '"')
			// 	{
			// 		char quote = crnt_str[i++];
			// 		int start = i;
			// 		while (crnt_str[i] && crnt_str[i] != quote)
			// 			i++;
			// 		if (!crnt_str[i])
			// 		{
			// 			printf("minishell: syntax error: unexpected EOL while looking for matching %c\n",quote);
			// 			free(final_str);
			// 			exit(1);
			// 		}
			// 		while (start < i)
			// 		final_str[j++] = crnt_str[start++];
			// 		i++;
			// 	}
			// 	while (crnt_str[i] && crnt_str[i] != ' ' && crnt_str[i] != '\t' && crnt_str[i] != '\'' && crnt_str[i] != '"')
			// 		final_str[j++] = crnt_str[i++];
			// 	final_str[j] = '\0';
			// 	res[k++] = ft_strdup(final_str);
			// 	free(final_str);
			// }
			// else
					// k++;