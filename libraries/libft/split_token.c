/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_token.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moirhira <moirhira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 20:55:19 by moirhira          #+#    #+#             */
/*   Updated: 2025/04/24 22:01:55 by moirhira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdio.h>


// static char	**free_split(char **res, size_t indx)
// {
// 	while (indx > 0)
// 	{
// 		free(res[--indx]);
// 	}
// 	free(res);
// 	return (NULL);
// }
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
char	**split_token(char *s)
{
	char	**res;
	int k = 0;
	int	i;
	int start;
	int end ;
	res = (char **)malloc(sizeof(char *) * 100);
	if (!res)
		return (NULL);
	i = 0;
	
	while (s[i])
	{
		while (s[i] == ' ' || s[i] == '\t')
			i++;
		if (s[i] == '\"' || s[i] == '\'')
		{
			char *quoted_str;
			quoted_str = malloc(ft_strlen(&s[i]) + 1);
			if (!quoted_str)
				exit(1);
			int j = 0;
			while (s[i] == '\'' || s[i] == '"')
			{
				char quote = s[i++];
				int start = i;
				while (s[i] && s[i] != quote)
					i++;
				if (!s[i])
				{
					printf("minishell: syntax error: unexpected EOL while looking for matching %c\n",quote);
					free(quoted_str);
					exit(1);
				}
				while (start < i)
					quoted_str[j++] = s[start++];
				i++;
			}
			while (s[i] && s[i] != ' ' && s[i] != '\t' && s[i] != '\'' && s[i] != '"')
				quoted_str[j++] = s[i++];
			quoted_str[j] = '\0';
			res[k++] = ft_strdup(quoted_str);
			free(quoted_str);
		}
		else
		{
			start = i;
			while (s[i] && s[i] != ' ' && s[i] != '\t')
				i++;
			end = i;
			while (s[i] == ' ' || s[i] == '\t')
				i++;
			res[k] = ft_memalloc(s, start, end);
			if(!res[k])
				exit(1);
			char *crnt_str = ft_strdup(res[k]);
			if(ft_strchr(crnt_str, '|')) 
				k = split_symbols(res, crnt_str, k, '|');
			else if(ft_strchr(crnt_str, '>')) 
				k = split_symbols(res, crnt_str, k, '>');
			else if(ft_strchr(crnt_str, '<')) 
				k = split_symbols(res, crnt_str, k, '<');
			else if (ft_strchr(crnt_str, '"') || ft_strchr(crnt_str, '\''))
			{
				int i = 0;
				int j = 0;
				char *final_str = malloc(ft_strlen(crnt_str));
				while (crnt_str[i] != '\'' && crnt_str[i] != '"')
				{
					final_str[j] = crnt_str[i];
					j++;
					i++;
				}
				while (crnt_str[i] == '\'' || crnt_str[i] == '"')
				{
					char quote = crnt_str[i++];
					int start = i;
					while (crnt_str[i] && crnt_str[i] != quote)
						i++;
					if (!crnt_str[i])
					{
						printf("minishell: syntax error: unexpected EOL while looking for matching %c\n",quote);
						free(final_str);
						exit(1);
					}
					while (start < i)
					final_str[j++] = crnt_str[start++];
					i++;
				}
				while (crnt_str[i] && crnt_str[i] != ' ' && crnt_str[i] != '\t' && crnt_str[i] != '\'' && crnt_str[i] != '"')
					final_str[j++] = crnt_str[i++];
				final_str[j] = '\0';
				res[k++] = ft_strdup(final_str);
				free(final_str);
			}
			else
					k++;
		}
		while (s[i] == ' ' || s[i] == '\t')
			i++;
	}
	return (res);
}


//check the env variabless