/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_command_in_path.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekhallaf <ekhallaf@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 06:48:46 by ekhallaf          #+#    #+#             */
/*   Updated: 2025/07/15 20:18:13 by ekhallaf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

////////////     HELPER    /////////////////

static int is_executable(const char *path)
{
    return (access(path, X_OK) == 0);
}

////////////////// GE THE VALUE OF A VARABLE //////////

// char *get_env_value(t_envp *env, const char *key)
// {
//     while (env)
//     {
//         if (strcmp(env->key, key) == 0)
//             return env->value;
//         env = env->next;
//     }
//     return NULL;
// }

// ft_strdup() - Custom string duplication (like strdup)

char *ft_strdup(const char *s)
{
    size_t len = strlen(s) + 1;
    char *dup = malloc(len);
    
    if (dup)
        memcpy(dup, s, len);
    return dup;
}


///////////////////////////  FIND_command_in_path   ///////////////////////////////////////////////////

char	*find_command_in_path(const char *cmd, t_envp *env)
{
	char	full[PATH_MAX], *dir, *path, *copy;
	if (!cmd)
		return (NULL);
	if (strchr(cmd, '/'))
		return (is_executable(cmd) ? ft_strdup(cmd) : NULL);
	path = get_env_value(env, "PATH");
	if (!path)
		return (NULL);
	copy = ft_strdup(path);
	if (!copy)
		return (NULL);
	dir = strtok(copy, ":");
	while (dir)
	{
		if (*dir && strlen(dir) + strlen(cmd) + 2 < PATH_MAX)
		{
			full[0] = '\0';
			strcat(full, dir);
			strcat(full, "/");
			strcat(full, cmd);
			if (is_executable(full))
				return (free(copy), ft_strdup(full));
		}
		dir = strtok(NULL, ":");
	}
	return (free(copy), NULL);
}



