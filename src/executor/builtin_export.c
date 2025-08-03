/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moirhira <moirhira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 09:49:31 by ekhallaf          #+#    #+#             */
/*   Updated: 2025/08/03 16:13:20 by moirhira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

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
            {
                if (update_env_var(env, key, value) != 0)
                {
                    display_error("export", "failed to update the env");
                    return (1);
                }
            }
            free(key);
            free(value);
        }
        else
        {
             if (!check_var(arg))
            {
                ft_putstr_fd("minishell: export: `", 2);
                ft_putstr_fd(arg, 2);
                ft_putstr_fd("': not a valid identifier\n", 2);
                return (exit_status(1));
            }
            if (update_env_var(env, arg, NULL) != 0)
            {
                display_error("export", "failed to update the env");
                return (1);
            }
        }
        i++;
    }
    return exit_status(0);
}