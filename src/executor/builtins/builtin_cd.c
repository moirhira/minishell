/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moirhira <moirhira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 06:48:21 by ekhallaf          #+#    #+#             */
/*   Updated: 2025/08/07 15:08:35 by moirhira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"


int builtin_cd(t_command *cmd, t_envp **env)
{
    char cwd[PATH_MAX];
    char *path;
    
    if (ft_strlen_2d(cmd->args) > 2)
    {
        display_error("cd", "too many arguments");
        return (1);
    }
    
    
    if (cmd->args[1] == NULL)
    {
        path = get_env_value(*env, "HOME");
        if (!path)
        {
            display_error("cd", "HOME not set");
            return (1);
        }
    }
    else
        path = cmd->args[1];
        
    
    if (getcwd(cwd, sizeof(cwd)) != NULL)
    {
        if (update_env_var(env, "OLDPWD", cwd) != 0)
        {
            display_error("cd", "failed to update OLDPWD");
            return (1);
        }
    }

    if (chdir(path) != 0)
    {
        if (cmd->args[1])
            display_error(cmd->args[1], strerror(errno));
        else
            display_error("HOME", strerror(errno));
        return (1);
    }

    if (getcwd(cwd, sizeof(cwd)) != NULL)
    {
        if (update_env_var(env, "PWD", cwd) != 0)
        {
            display_error("cd", "failed to update PWD");
            return (1);
        }
    }

    return (0);
}
