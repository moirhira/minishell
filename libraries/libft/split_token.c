/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_token.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moirhira <moirhira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 20:55:19 by moirhira          #+#    #+#             */
/*   Updated: 2025/04/22 23:12:05 by moirhira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdio.h>

// counting the number of words
// static int	ft_ctrword(char const *s, char c)
// {
// 	size_t	wordctr;
// 	size_t	i;

// 	if (s == NULL)
// 		return (0);
// 	i = 0;
// 	wordctr = 0;
// 	while (s[i] != '\0')
// 	{
// 		if (s[i] != c && (i == 0 || c == s[i - 1]))
// 			wordctr++;
// 		i++;
// 	}
// 	return (wordctr);
// }

// // Allocate Memory for the Array of Substrings


// static char	*copy_word(const char **s, char c)
// {
// 	int i;
// 	const char	*start;
// 	size_t		len;

// 	start = *s;
// 	len = 0;
// 	i = 0;
// 	while (*s[i] && *s[i] != c)
// 	{
// 		if (*s[i] == '\"')
// 		{
// 			while (*s[i] != '\"')
// 			{
// 				*s[i++];
// 				len++;
// 			}
// 			break;
// 		}
// 		len++;
// 		*s[i]++;
// 	}
// 	return (ft_memalloc(start, len));
// }

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
				printf("minishell: syntax error: unexpected EOF while looking for matching '\"' \n");
				exit(1);
			}
			start = i;
			while (s[i] && s[i] != quote)
				i++;
			res[k++] = ft_memalloc(s, start, i);
			if (s[i] == quote)
				i++;
		}
		// else if (s[i] == '|')
		// {
		// 	printf("here\n");
		// 	// char *str_befor;
		// 	// char *str_after;
		// 	// while (s[i] != ' ' && s[i] != '\t')
		// 	// 	i--;
		// 	// printf("char :%c\n", s[i]);
		// }
		else
		{
			start = i;
			while (s[i] && s[i] != ' ' && s[i] != '\t')
				i++;
			end = i;
			while (s[i] == ' ' || s[i] == '\t')
				i++;
			res[k] = ft_memalloc(s, start, end);
			char *crnt_str = ft_strdup(res[k]);
			if(ft_strchr(crnt_str, '|')) 
        	{
				int x = ft_strlen(crnt_str) - 1;
				char *str_befor;
				// char *str_after;
				while (crnt_str[x] != ' ' && crnt_str[x] != '\t' && x >= 0)
				{
					// printf("len and char : %d %c\n",x, crnt_str[x]);
					x--;
				}
				while (crnt_str[x] != '|')
					*str_befor++ = crnt_str[x++];
				printf("tttttttttttttttttttttttt %s\n", str_befor);
				// printf("len before %d", j);
				// printf("char :%c\n", arr_commands[i][j]);
			}
			k++;
		}
		while (s[i] == ' ' || s[i] == '\t')
			i++;
	}
	return (res);
}
