/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekhallaf <ekhallaf@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 14:53:22 by moirhira          #+#    #+#             */
/*   Updated: 2025/07/27 20:38:17 by ekhallaf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# ifndef LIBFT_H
# define LIBFT_H

# include <string.h>
# include <unistd.h>
# include <stdlib.h>


char	*ft_strcat(char *dest, const char *src);
int	ft_strlen(const char *str);
char	**ft_split(char const *s, char c);
char	*ft_strchr(const char *str, int ch);
char	*ft_strdup(const char *str);
int	ft_strcmp(const char *s1, const char *s2);
size_t	ft_strlcpy(char *dst, char *src, size_t size);
int	ft_isalnum(int c);
char	*ft_substr(char const *s, unsigned int start, size_t len);
char	*ft_strjoin(char const *s1, char const *s2);
void	*ft_calloc(size_t count, size_t size);
void	ft_bzero(void *s, size_t n);
int	ft_strncmp(const char *s1, const char *s2, size_t n);
int	ft_strlen_2d(char **str);
char	*ft_itoa(int n);
void	ft_putstr_fd(char *s, int fd);
int ft_isspace(char c);
size_t	ft_strlcat(char *dst, const char *src, size_t dstsize);
char	*ft_strcpy(char *dest, const char *src);
char	*ft_strtok(char *str, const char *delim);
# endif