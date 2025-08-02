/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_command_in_path.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moirhira <moirhira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 06:48:46 by ekhallaf          #+#    #+#             */
/*   Updated: 2025/08/01 16:14:14 by moirhira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"


static int is_file_exists(const char *path)
{
    struct stat st;
    return (stat(path, &st) == 0);
}
static int is_executable(const char *path)
{
    struct stat st;
    if (stat(path, &st) != 0)
        return (0);
    return (access(path, X_OK) == 0);
}


char *find_command_in_path(const char *cmd, t_envp *env, int *status)
{
    char *path_env = get_env_value(env, "PATH");
    char full[PATH_MAX];

    if (!cmd || !*cmd)
        return (NULL);

    if (strchr(cmd, '/'))
    {
        if (!is_file_exists(cmd))
        {
            ft_putstr_fd("minishell: ", 2);
            perror(cmd);
            *status = 127;
            return (NULL);
        }
        if(!is_executable(cmd))
        {
            ft_putstr_fd("minishell: ", 2);
            perror(cmd);
            *status = 126;
            return (NULL);
        }
        return (ft_strdup(cmd));
    }

    if (!path_env)
        return (NULL);

    char **paths = ft_split_advanced(path_env, ":");
    if (!paths)
        return (NULL);

    int i = 0;
    while (paths[i])
    {
        snprintf(full, sizeof(full), "%s/%s", paths[i], cmd);
        if (is_executable(full))
        {
            char *res = ft_strdup(full);
            free_array(paths, ft_strlen_2d(paths));
            return (res);
        }
        i++;
    }
    free_array(paths, ft_strlen_2d(paths));
    return (NULL);
}




