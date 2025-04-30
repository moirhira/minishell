/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moirhira <moirhira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 15:50:34 by moirhira          #+#    #+#             */
/*   Updated: 2025/04/30 15:37:27 by moirhira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_token *create_token(char *str, int type, int is_attached)
{
    t_token *new;
    new = (t_token *)malloc(sizeof(t_token));
    if (!new)
        return (NULL);
    new->value = ft_strdup(str);
    new->type = type;
	new->attached = is_attached;
    new->next = NULL;
    return (new);
}
void add_token(t_token **token_lst, t_token *new_token)
{
    t_token *ptr;
    if (!*token_lst)
    {
        *token_lst = new_token;
        return;
    }
    ptr = *token_lst;
    while (ptr->next)
        ptr = ptr->next;
    ptr->next = new_token;
}
char *get_env_value(char **my_env, const char *var_name)
{
	int i = 0;
	size_t len = ft_strlen(var_name);
	
	while (my_env[i])
	{
		if (ft_strncmp(my_env[i], var_name, len) == 0 && my_env[i][len] == '=')
			return(my_env[i] + len + 1);
		i++;

	}
	return (NULL);
}

int was_previous_space(char *s, int i)
{
    if (i == 0)
    {
        return (0);
    }
	if (s[i - 1] == ' ' || s[i - 1] == '\t')
    {
		return (0);
    }
	return (1);
}

t_token *get_last_token(t_token *lst)
{
    while (lst && lst->next)
        lst = lst->next;
    return (lst);
}
