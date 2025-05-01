/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moirhira <moirhira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 20:52:43 by moirhira          #+#    #+#             */
/*   Updated: 2025/05/01 20:52:55 by moirhira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	fillstr(char *res, int n, int *len)
{
	while (n != 0)
	{
		res[--(*len)] = (n % 10) + '0';
		n /= 10;
	}
}

static int	ft_countdgt(int n)
{
	size_t	ctr;

	if (n <= 0)
		ctr = 1;
	else
		ctr = 0;
	while (n != 0)
	{
		n /= 10;
		ctr++;
	}
	return (ctr);
}

char	*ft_itoa(int n)
{
	char	*res;
	int		len;

	len = ft_countdgt(n);
	if (n == -2147483648)
		return (ft_strdup("-2147483648"));
	res = (char *)malloc(sizeof(char) * (len + 1));
	if (!res)
		return (NULL);
	res[len] = '\0';
	if (n == 0)
	{
		res[0] = '0';
		return (res);
	}
	if (n < 0)
	{
		res[0] = '-';
		n = -n;
	}
	fillstr(res, n, &len);
	return (res);
}