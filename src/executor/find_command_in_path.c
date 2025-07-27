/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_command_in_path.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moirhira <moirhira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 06:48:46 by ekhallaf          #+#    #+#             */
/*   Updated: 2025/07/27 17:58:29 by moirhira         ###   ########.fr       */
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

char *find_command_in_path(const char *cmd, t_envp *env)
{
    char *path_env = get_env_value(env, "PATH");
    char *copy, *dir, full[PATH_MAX];

    if (!cmd || !*cmd)
        return (NULL);

    // Case: absolute or relative path like ./a.out or /bin/ls
    if (strchr(cmd, '/'))
        return (is_executable(cmd) ? ft_strdup(cmd) : NULL);

    // Case: command like "ls"
    if (!path_env)
        return (NULL); // no PATH

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




