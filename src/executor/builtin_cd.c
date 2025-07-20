/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekhallaf <ekhallaf@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 06:48:21 by ekhallaf          #+#    #+#             */
/*   Updated: 2025/07/15 19:54:24 by ekhallaf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int builtin_cd(t_command *cmd, t_envp **env)
{
    (void)env;
    char *path = cmd->args[1];
    char cwd[PATH_MAX];
    char *old_pwd = getcwd(cwd, sizeof(cwd));

    if (chdir(path) != 0)
    {
        perror("minishell: cd");
        return exit_status(1);  ///Set failure status
    }

    setenv("OLDPWD", old_pwd, 1);
    getcwd(cwd, sizeof(cwd));
    setenv("PWD", cwd, 1);

    return exit_status(0);  ///Set success status
}


// unecessary but i use it when testing 
// if (!path || strcmp(path, "~") == 0) 
// {
//     if (!(path = getenv("HOME")))
//     {
//         printf("minishell: cd: HOME not set\n");
//         g_last_exit_status = 1;
//         return;
//     }
// }
// else if (strcmp(path, "-") == 0)
// {
//     if (!(path = getenv("OLDPWD")))
//     {    
//         printf(stderr, "minishell: cd: OLDPWD not set\n");
//         g_last_exit_status = 1;
//         return;
//     }
// }