/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_token.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moirhira <moirhira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 20:55:19 by moirhira          #+#    #+#             */
/*   Updated: 2025/04/23 22:13:49 by moirhira         ###   ########.fr       */
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
	char quote;
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
			quote = s[i];
			i++;
			char *string = &s[i];
			if (!ft_strchr(string, quote))
			{
				printf("minishell: syntax error: unexpected EOL while looking for matching \"\n");
				exit(1);
			}
			start = i;
			while (s[i] && s[i] != quote)
				i++;
			res[k++] = ft_memalloc(s, start, i);
			if (s[i] == quote)
				i++;
		}
		else
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
			else
				k++;
		}
		while (s[i] == ' ' || s[i] == '\t')
			i++;
	}
	return (res);
}
