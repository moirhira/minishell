/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_command_in_path.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moirhira <moirhira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 06:48:46 by ekhallaf          #+#    #+#             */
/*   Updated: 2025/08/11 11:45:11 by moirhira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"


char	*ft_strjoin_path(const char *dir, const char *cmd)
{
	char	*full;
	size_t	len_dir = ft_strlen(dir);
	size_t	len_cmd = ft_strlen(cmd);

	full = ft_malloc(len_dir + 1 + len_cmd + 1);
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

static char *handel_absolute_path(char *cmd, int *status)
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
        display_error(cmd, "Permission denied");
        *status = 126;
        return (NULL);
    }
    return (ft_strdup(cmd));
}

static char*handel_relative_path(char *path_env, char *cmd, int *status, char **paths)
{
    int i;
    char *full;
    char *result = NULL;
   
    i = 0;
    while (paths[i])
    {
        full = ft_strjoin_path(paths[i], cmd);
        if (!full)
            return (NULL);
        if (is_file_exists(full))
        {
            if(is_directory(full))
            {
                i++;
                continue;
            }
            if (!is_executable(full))
            {
                display_error(cmd, "Permission denied");
                *status = 126;
                return (NULL);
            }
            result = full;
            break;
        }
        i++;
    }
    if (!result)
    {
        display_error(cmd, "Command not found");
        *status = 127;
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
        return (handel_absolute_path(cmd, status));
    
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
    return (handel_relative_path(path_env, cmd, status, paths));
}




