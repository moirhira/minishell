/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcat.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moirhira <moirhira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 15:04:27 by moirhira          #+#    #+#             */
/*   Updated: 2025/04/21 14:06:41 by moirhira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strcat(char *dst, const char src)
{
	size_t	dlen;

	dlen = ft_strlen(dst);
	dst[dlen + 1] = src;
	dst[dlen + 1] = '\0';
	return (dlen);
}