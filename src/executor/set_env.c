/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_env.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekhallaf <ekhallaf@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 21:50:06 by ekhallaf          #+#    #+#             */
/*   Updated: 2025/08/02 21:50:35 by ekhallaf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_envp  *find_env(t_envp *env, const char *key)
{
    while (env)
    {
        if (ft_strcmp(env->key, key) == 0)
            return (env);
        env = env->next;
    }
    return (NULL);
}

int set_env(t_envp **env, const char *key, const char *value)
{
    t_envp *node;

    if (!key || !value)
        return (1);
    node = find_env(*env, key);
    if (node)
    {
        free(node->value);
        node->value = ft_strdup(value);
        return (0);
    }
    node = malloc(sizeof(t_envp));
    if (!node)
        return (1);
    node->key = ft_strdup(key);
    node->value = ft_strdup(value);
    node->next = *env;
    *env = node;
    return (0);
}
