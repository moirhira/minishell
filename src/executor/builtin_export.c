/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moirhira <moirhira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 09:49:31 by ekhallaf          #+#    #+#             */
/*   Updated: 2025/08/01 13:29:53 by moirhira         ###   ########.fr       */
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

int check_var(char *str)
{
    int i;
    
    i = 1;
    while (str[i])
    {
        if(!ft_isalpha(str[0]) && str[0] != '_')
            return(0);
        if (!ft_isalnum(str[i]) && str[i] != '_')
            return (0);
        i++;
    }
    return (1);
}

int builtin_export(t_command *cmd, t_envp **env)
{
     if (!cmd->args[1])
    {
        print_sorted_export(*env);
        return exit_status(0);
    }
    
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
            if (!check_var(key))
            {
                ft_putstr_fd("minishell: export: `", 2);
                ft_putstr_fd(key, 2);
                ft_putstr_fd("=", 2);
                ft_putstr_fd(value, 2);
                ft_putstr_fd("': not a valid identifier\n", 2);
                free(key);
                free(value);
                return (exit_status(1));
            }
            if (key && value)
                update_env_var(env, key, value);

            free(key);
            free(value);
        }
        else
        {
             if (!check_var(arg))
            {
                ft_putstr_fd("minishell: expooooort: `", 2);
                ft_putstr_fd(arg, 2);
                ft_putstr_fd("': not a valid identifier\n", 2);
                return (exit_status(1));
            }
            update_env_var(env, arg, NULL);
        }
        i++;
    }
    return exit_status(0);
}