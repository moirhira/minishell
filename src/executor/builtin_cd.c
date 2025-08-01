/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moirhira <moirhira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 06:48:21 by ekhallaf          #+#    #+#             */
/*   Updated: 2025/08/01 21:46:18 by moirhira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"


int builtin_cd(t_command *cmd, t_envp **env)
{
    char cwd[PATH_MAX];
    char *path;
    
    if (ft_strlen_2d(cmd->args) > 2)
    {
        ft_putstr_fd("minishell: cd: too many arguments\n", 2);
        return (1);
    }
    
    if (getcwd(cwd, sizeof(cwd)) != NULL)
    {
        if (update_env_var(env, "OLDPWD", cwd) != 0)
        {
            ft_putstr_fd("minishell: cd: failed to update OLDPWD\n", 2);
            return (1);
        }
    }
        
    if (cmd->args[1] == NULL)
    {
        path = get_env_value(*env, "HOME");
        if (!path)
        {
            ft_putstr_fd("minishell: cd: HOME not set\n", 2);
            return (1);
        }
    }
    else if (ft_strcmp(cmd->args[1], "-") == 0)
    {
        path = get_env_value(*env, "OLDPWD");
         if (!path)
        {
            ft_putstr_fd("minishell: cd: HOME not set\n", 2);
            return (1);
        }
        printf("%s\n", path);
    }
    else
        path = cmd->args[1];
        
        
    if (chdir(path) != 0)
    {
        ft_putstr_fd("minishell: cd: ", 2);
        if (cmd->args[1])
            perror(cmd->args[1]);
        else
            perror("HOME");
        return (1);
    }

    if (getcwd(cwd, sizeof(cwd)) != NULL)
    {
        if (update_env_var(env, "PWD", cwd) != 0)
        {
            ft_putstr_fd("minishell: cd: failed to update OLDPWD\n", 2);
            return (1);
        }
    }

    return (0);
}
