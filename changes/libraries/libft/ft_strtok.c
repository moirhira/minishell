/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtok.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekhallaf <ekhallaf@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 20:37:57 by ekhallaf          #+#    #+#             */
/*   Updated: 2025/07/27 20:38:04 by ekhallaf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strtok(char *str, const char *delim)
{
	static char	*next;
	char		*start;

	if (str)
		next = str;
	if (!next)
		return (NULL);
	while (*next && ft_strchr(delim, *next))
		next++;
	if (!*next)
		return (NULL);
	start = next;
	while (*next && !ft_strchr(delim, *next))
		next++;
	if (*next)
	{
		*next = '\0';
		next++;
	}
	return (start);
}