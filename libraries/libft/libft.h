/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moirhira <moirhira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 14:53:22 by moirhira          #+#    #+#             */
/*   Updated: 2025/04/21 15:38:12 by moirhira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# ifndef LIBFT_H
# define LIBFT_H

# include <string.h>
# include <unistd.h>
# include <stdlib.h>


size_t	ft_strcat(char *dst, const char src);
int	ft_strlen(const char *str);
char	**ft_split(char const *s, char c);
char	*ft_strchr(const char *str, int ch);
char	*ft_strdup(const char *str);

# endif