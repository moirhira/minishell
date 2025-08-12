/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_operations.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moirhira <moirhira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 19:28:40 by moirhira          #+#    #+#             */
/*   Updated: 2025/08/12 20:48:45 by moirhira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"


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

t_envp *new_env_node(char *key, char *value)
{
    t_envp *new_node = ft_malloc(sizeof(t_envp));
    new_node->key = ft_strdup(key);
    if (value)
        new_node->value = ft_strdup(value);
    else
        new_node->value = NULL;
    new_node->next = NULL;
    return (new_node);
}

void append_env(t_envp **env, t_envp *node)
{
    t_envp *last;
    
    last = ft_lstlast(*env);
    if (last)
        last->next = node;
    else
        *env = node;

}
int  update_env_var(t_envp **env, char *key, char *value)
{
    t_envp *tmp;
    
    if (!env || !key)
        return (-1);
    tmp = *env;
    while (tmp)
    {
        if (ft_strcmp(tmp->key, key) == 0)
        {
            if (value)
                tmp->value = ft_strdup(value);
            else
                tmp->value = NULL;
            return (0);
        }
        tmp = tmp->next;
    }
    append_env(env, new_env_node(key, value));
    return (0);
}

