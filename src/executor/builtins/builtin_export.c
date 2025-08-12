/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moirhira <moirhira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 09:49:31 by ekhallaf          #+#    #+#             */
/*   Updated: 2025/08/12 13:34:28 by moirhira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

int check_var(char *str, char *arg)
{
    int i;
    
    if(!str || (!ft_isalpha(str[0]) && str[0] != '_'))
    {
        ft_putstr_fd("minishell: export: `", 2);
        ft_putstr_fd(arg, 2);
        ft_putstr_fd("': not a valid identifier\n", 2);
        return(0);
    }
    i = 1;
    while (str[i])
    {
        
        if (!ft_isalnum(str[i]) && str[i] != '_')
        {
            ft_putstr_fd("minishell: export: `", 2);
            ft_putstr_fd(arg, 2);
            ft_putstr_fd("': not a valid identifier\n", 2);
            return (0);
        }
        i++;
    }
    return (1);
}

int handle_export_arg(char *arg, t_envp **env)
{
    char *key;
    char *value;
    char *equal_sign;
    
    equal_sign = ft_strchr(arg, '=');
    key = NULL;
    value = NULL;
    if (equal_sign)
    {
        key = ft_strndup(arg, equal_sign - arg);
        value = ft_strdup(equal_sign + 1);
    }
    else
        key = ft_strdup(arg);
    if (!check_var(key, arg))
    {
        return (1);
    }
    else
    {
        if (update_env_var(env, key, value) != 0)
            return (display_error("export", "failed to update the env"), 1);
    }
    return (0);
}


int builtin_export(t_command *cmd, t_envp **env)
{
    int i;
    int final_status;
    
    if (!cmd->args[1])
        return (print_sorted_export(*env), 0);
    if (!env || !*env)
        return(1);
    i = 1;
    final_status = 0;
    while (cmd->args[i])
    {
        if (handle_export_arg(cmd->args[i], env) != 0)
            final_status = 1;
        i++;
    }
    return (final_status);
}

