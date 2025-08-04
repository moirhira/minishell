/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_operations.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moirhira <moirhira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 19:28:40 by moirhira          #+#    #+#             */
/*   Updated: 2025/08/04 19:22:59 by moirhira         ###   ########.fr       */
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
int  update_env_var(t_envp **env, char *key, char *value)
{
    if (!env || !key)
        return (-1);

    t_envp *tmp = *env;
    t_envp *last = NULL;

    while (tmp)
    {
        if (ft_strcmp(tmp->key, key) == 0)
        {
            // free(tmp->value);
            if (value)
            {
                tmp->value = ft_strdup(value);
                if (!tmp->value)
                    return (-1);
            }
            else
                tmp->value = NULL;
            return (0);
        }
        tmp = tmp->next;
    }

    t_envp *new_node = ft_malloc(sizeof(t_envp));
    if (!new_node)
        return (-1);

    new_node->key = ft_strdup(key);
    if (value)
    {
        new_node->value = ft_strdup(value);
        if (!new_node->value)
        {
            // free(new_node->key);
            // free(new_node);
            return (-1);
        }
    }
    else
        new_node->value = NULL;
        
    new_node->next = NULL;

    last = ft_lstlast(*env);
    if (last)
        last->next = new_node;
    else
        *env = new_node;
    return (0);
}