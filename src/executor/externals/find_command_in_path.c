/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_command_in_path.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moirhira <moirhira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 06:48:46 by ekhallaf          #+#    #+#             */
/*   Updated: 2025/08/12 11:33:53 by moirhira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

static char *handle_absolute_path(char *cmd, int *status)
{
    if (!is_file_exists(cmd))
    {
        display_error(cmd, "No such file or directory");
        *status = 127;
        return (NULL);
    }
    if (is_directory(cmd))
    {
        display_error(cmd, "Is a directory");
        *status = 126;
        return (NULL);
    }
    if(!is_executable(cmd))
    {
        display_error(cmd, "Permmission denied");
        *status = 126;
        return (NULL);
    }
    return (ft_strdup(cmd));
}

static char *find_exec_in_path(char **paths, char *cmd, int *status)
{
    int i;
    char *full;
    int found_no_perms;
    
    i = 0;
    found_no_perms = 0;
    while (paths[i])
    {
        full = ft_strjoin_path(paths[i], cmd);
        if (is_file_exists(full))
        {
            if(is_directory(full))
            {
                i++;
                continue;
            }
            if (is_executable(full))
                return (full);
            else
            {
                found_no_perms = 1;
                i++;
                continue;
            }
        }
        i++;
    }
    if (found_no_perms)
        *status = 126;
    else
        *status = 127;
    return (NULL);
}

static char *handle_relative_path(char *cmd, int *status, char **paths)
{
    char *result;
   
    result = find_exec_in_path(paths, cmd, status);
    if (!result)
    {
        if (*status == 127)
            display_error(cmd, "command not found");
        else if (*status == 126)
            display_error(cmd, "Permission denied");
    }
    return (result);
}


char *find_command_in_path(char *cmd, t_envp *env, int *status)
{
    char *path_env;
    char **paths;

    if (!cmd || !*cmd)
        return (NULL);
        
    if (ft_strchr(cmd, '/'))
        return (handle_absolute_path(cmd, status));
    path_env = get_env_value(env, "PATH");
    if (!path_env || !*path_env)
    {
        display_error(cmd, "No such file or directory");
        *status = 127;
        return (NULL);
    }
    paths = ft_split_advanced(path_env, ":");
    if (!paths)
        return (NULL);
    return (handle_relative_path(cmd, status, paths));
}
