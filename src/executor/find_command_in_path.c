/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_command_in_path.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moirhira <moirhira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 06:48:46 by ekhallaf          #+#    #+#             */
/*   Updated: 2025/08/01 09:47:08 by moirhira         ###   ########.fr       */
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
    char *copy, *dir, full[PATH_MAX];

    if (!cmd || !*cmd)
        return (NULL);

    if (strchr(cmd, '/'))
    {
        if (!is_file_exists(cmd))
        {
            perror("minishell");
            *status = 127;
            return (NULL);
        }
        if(!is_executable(cmd))
        {
            perror("minishell");
            *status = 126;
            return (NULL);
        }
    }

    if (!path_env)
        return (NULL);

    copy = ft_strdup(path_env);
    if (!copy)
        return (NULL);

    dir = strtok(copy, ":");
    while (dir)
    {
        snprintf(full, sizeof(full), "%s/%s", dir, cmd);
        if (is_executable(full))
        {
            char *res = ft_strdup(full);
            free(copy);
            return (res);
        }
        dir = strtok(NULL, ":");
    }
    free(copy);
    return (NULL);
}




