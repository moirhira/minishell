/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstlast.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moirhira <moirhira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 09:24:58 by moirhira          #+#    #+#             */
/*   Updated: 2025/08/12 09:25:23 by moirhira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "libft.h"


t_envp	*ft_lstlast(t_envp *lst)
{
	if (!lst)
		return (NULL);
	while (lst->next)
	{
		lst = lst->next;
	}
	return (lst);
}