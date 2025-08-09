/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_command_in_path.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moirhira <moirhira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 06:48:46 by ekhallaf          #+#    #+#             */
/*   Updated: 2025/08/09 14:44:59 by moirhira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"


char	*ft_strjoin_path(const char *dir, const char *cmd)
{
	char	*full;
	size_t	len_dir = ft_strlen(dir);
	size_t	len_cmd = ft_strlen(cmd);

	full = ft_malloc(len_dir + 1 + len_cmd + 1);
	if (!full)
		return (NULL);
	ft_strcpy(full, dir);
	full[len_dir] = '/';
	ft_strcpy(full + len_dir + 1, cmd);
	return (full);
}

static int is_file_exists(const char *path)
{
    struct stat st;
    return (stat(path, &st) == 0);
}
static int is_executable(const char *path)
{
    return (access(path, X_OK) == 0);
}

static int is_directory(const char *path)
{
    struct stat st;
    if (stat(path, &st) == -1)
        return (0);
    return (S_ISDIR(st.st_mode));
}


char *find_command_in_path(char *cmd, t_envp *env, int *status)
{
    char *path_env = get_env_value(env, "PATH");
    if (!path_env)
        return ft_strdup(cmd);
    char *full;

    if (!cmd || !*cmd)
        return (NULL);

    if (strchr(cmd, '/'))
    {
        if (!is_file_exists(cmd))
        {
            display_error(cmd, strerror(errno));
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
            display_error(cmd, strerror(errno));
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
        full = ft_strjoin_path(paths[i], cmd);
        if (!full)
            return (NULL);
        if(is_directory(full))
        {
            i++;
            continue;
        }
        if (is_executable(full)) // check if exist first then check permission.
        {
            char *res = ft_strdup(full);
            return (res);
        }
        i++;
    }
    return (NULL);
}




