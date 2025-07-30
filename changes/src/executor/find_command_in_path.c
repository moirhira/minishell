/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_command_in_path.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekhallaf <ekhallaf@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 06:48:46 by ekhallaf          #+#    #+#             */
/*   Updated: 2025/07/27 21:30:40 by ekhallaf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int is_executable(const char *path)
{
    return (access(path, X_OK) == 0);
}

static char *check_direct_path(const char *cmd)
{
    if (ft_strchr(cmd, '/'))
    {
        if (is_executable(cmd))
            return ft_strdup(cmd);
        return NULL;
    }
    return NULL;
}


static char *search_in_path(const char *cmd, char *path_env)
{
    char *copy;
    char *dir;
    char full[PATH_MAX];

    copy = ft_strdup(path_env);
    if (!copy)
        return NULL;
    dir = ft_strtok(copy, ":");
    while (dir)
    {
        ft_strcpy(full, dir);
        ft_strcat(full, "/");
        ft_strcat(full, cmd);
        if (is_executable(full))
        {
            char *res = ft_strdup(full);
            free(copy);
            return res;
        }
        dir = ft_strtok(NULL, ":");
    }
    free(copy);
    return NULL;
}


char *find_command_in_path(const char *cmd, t_envp *env)
{
    char *path_env;
    char *result;

    if (!cmd || !*cmd)
        return NULL;
    result = check_direct_path(cmd);
    if (result)
        return result;
    path_env = get_env_value(env, "PATH");
    if (!path_env)
        return NULL;
    return search_in_path(cmd, path_env);
}

