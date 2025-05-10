/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moirhira <moirhira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 15:37:35 by moirhira          #+#    #+#             */
/*   Updated: 2025/05/09 20:48:01 by moirhira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *str)
{
	int		size;
	char	*ptr;
	int		i;

	i = 0;
	size = ft_strlen(str);
	ptr = (char *)malloc((size + 1) * sizeof(char));
	if (!ptr)
		return (NULL);
	while (i < size)
	{
		ptr[i] = str[i];
		i++;
	}
	ptr[size] = '\0';
	return (ptr);
}