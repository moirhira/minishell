/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moirhira <moirhira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 06:48:21 by ekhallaf          #+#    #+#             */
/*   Updated: 2025/08/11 19:43:55 by moirhira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

static int cd_helper(t_command *cmd, char *path, t_envp **env)
{
    char cwd[PATH_MAX];

    if (cmd->args[1] && cmd->args[1][0] == '\0')
        return (0);
    if (getcwd(cwd, sizeof(cwd)) != NULL)
    {
        if (update_env_var(env, "OLDPWD", cwd) != 0) 
            return(display_error("cd", "failed to update OLDPWD"), 1);
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
            return(display_error("cd", "failed to update PWD"), 1);
    }
    return (0);
}


int builtin_cd(t_command *cmd, t_envp **env)
{
    
    char *path;
    
    if (ft_strlen_2d(cmd->args) > 2)
        return(display_error("cd", "too many arguments"), 1);
    if (cmd->args[1])
        path = cmd->args[1];
    else
    {
        path = get_env_value(*env, "HOME");
        if (!path)
            return(display_error("cd", "HOME not set"), 1);
    }
    if (cd_helper(cmd, path, env) == 1)
        return (1);
    return (0);
}
