/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moirhira <moirhira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 14:53:22 by moirhira          #+#    #+#             */
/*   Updated: 2025/04/24 10:22:44 by moirhira         ###   ########.fr       */
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
char	**split_token(char *s);
int	ft_strcmp(const char *s1, const char *s2);
size_t	ft_strlcpy(char *dst, char *src, size_t size);

# endif