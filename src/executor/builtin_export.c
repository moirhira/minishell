/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekhallaf <ekhallaf@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 09:49:31 by ekhallaf          #+#    #+#             */
/*   Updated: 2025/07/16 00:19:47 by ekhallaf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"


void update_env_var(t_envp **env, char *key, char *value)
{
    if (!env || !key)
        return;

    t_envp *tmp = *env;

    while (tmp)
    {
        if (strcmp(tmp->key, key) == 0)
        {
            free(tmp->value);
            tmp->value = value ? strdup(value) : NULL;
            return;
        }
        tmp = tmp->next;
    }

    t_envp *new_node = malloc(sizeof(t_envp));
    if (!new_node)
        return;

    new_node->key = strdup(key);
    new_node->value = value ? strdup(value) : NULL;
    new_node->next = NULL;

    if (*env == NULL)
    {
        *env = new_node;
    }
    else
    {
        tmp = *env;
        while (tmp->next)
            tmp = tmp->next;
        tmp->next = new_node;
    }
}

int builtin_export(t_command *cmd, t_envp **env)
{
    if (!env || !*env)
        return exit_status(1);

    int i = 1;

    while (cmd->args[i])
    {
        char *arg = cmd->args[i];
        char *equal_sign = strchr(arg, '=');

        if (equal_sign)
        {
            int key_len = equal_sign - arg;
            char *key = strndup(arg, key_len);
            char *value = strdup(equal_sign + 1);

            if (key && value)
                update_env_var(env, key, value);

            free(key);
            free(value);
        }
        else
        {
            update_env_var(env, arg, NULL);
        }
        i++;
    }
    return exit_status(0);
}




