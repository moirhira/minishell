/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_advanced.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moirhira <moirhira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 20:41:26 by moirhira          #+#    #+#             */
/*   Updated: 2025/07/31 20:43:47 by moirhira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int is_delimiter(char ch, const char *charset)
{
    int i = 0;
    while (charset[i])
    {
        if (ch == charset[i])
            return (1);
        i++;
    }
    return (0);
}

static int ft_ctrword(char const *s, const char *charset)
{
    size_t  wordctr = 0;
    size_t  i = 0;

    if (!s)
        return (0);
    while (s[i])
    {
        if (!is_delimiter(s[i], charset) && (i == 0 || is_delimiter(s[i - 1], charset)))
            wordctr++;
        i++;
    }
    return (wordctr);
}

static char *copy_word(const char **s, const char *charset)
{
    const char  *start;
    size_t      len;
    size_t      i;
    char        *ptr;

    start = *s;
    len = 0;
    while (**s && !is_delimiter(**s, charset))
    {
        len++;
        (*s)++;
    }

    ptr = (char *)malloc(sizeof(char) * (len + 1));
    if (!ptr)
        return (NULL);
    i = 0;
    while (i < len)
    {
        ptr[i] = start[i];
        i++;
    }
    ptr[len] = '\0';
    return (ptr);
}

static char **free_split(char **res)
{
    size_t i = 0;
    while(res[i])
    {
        free(res[i]);
        i++;
    }
    free(res);
    return (NULL);
}

char **ft_split_advanced(char const *s, const char *charset)
{
    char    **res;
    size_t  i;

    if (!s)
        return (NULL);

    res = (char **)malloc(sizeof(char *) * (ft_ctrword(s, charset) + 1));
    if (!res)
        return (NULL);
    
    i = 0;
    while (*s)
    {
        while (*s && is_delimiter(*s, charset))
            s++;
        if (*s)
        {
            res[i] = copy_word(&s, charset);
            if (!res[i])
                return (free_split(res));
            i++;
        }
    }
    res[i] = NULL;
    return (res);
}