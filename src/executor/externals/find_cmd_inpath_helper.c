/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_cmd_inpath_helper.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moirhira <moirhira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 17:08:54 by moirhira          #+#    #+#             */
/*   Updated: 2025/08/11 17:10:56 by moirhira         ###   ########.fr       */
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

int is_file_exists(const char *path)
{
    struct stat st;
    return (stat(path, &st) == 0);
}
int is_executable(const char *path)
{
    return (access(path, X_OK) == 0);
}

int is_directory(const char *path)
{
    struct stat st;
    if (stat(path, &st) == -1)
        return (0);
    return (S_ISDIR(st.st_mode));
}