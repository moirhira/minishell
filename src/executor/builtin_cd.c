/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekhallaf <ekhallaf@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 06:48:21 by ekhallaf          #+#    #+#             */
/*   Updated: 2025/08/02 21:56:29 by ekhallaf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// int builtin_cd(t_command *cmd, t_envp **env)
// {
//     (void)env;
//     char *path = cmd->args[1];
//     char cwd[PATH_MAX];
//     char *old_pwd = getcwd(cwd, sizeof(cwd));

//     if (chdir(path) != 0)
//     {
//         perror("minishell: cd");
//         return exit_status(1);  /// Set failure status
//     }

//     setenv("OLDPWD", old_pwd, 1);
//     getcwd(cwd, sizeof(cwd));
//     setenv("PWD", cwd, 1);

//     return exit_status(0);  /// Set success status
// }

int builtin_cd(t_command *cmd, t_envp **env)
{
    char cwd[PATH_MAX];
    char *path;

    if (ft_strlen_2d(cmd->args) != 2)
    {
        ft_putstr_fd("minishell: ", 2);
        ft_putstr_fd(cmd->args[0], 2);
        ft_putstr_fd(": too many arguments\n", 2);
        return (1);
    }
    path = cmd->args[1];
    if (getcwd(cwd, sizeof(cwd)) != NULL)
        set_env(env, "OLDPWD", cwd);
    else
        set_env(env, "OLDPWD", "/home");

    if (chdir(path) != 0)
    {
        perror("cd");
        return (1);
    }
    if (getcwd(cwd, sizeof(cwd)) != NULL)
        set_env(env, "PWD", cwd);
    else
        set_env(env, "PWD", "/home");

    return (0);
}


//getcwd may fail in case; 
/// the current dir doesn't exists 
/// path too long that the path max 
/// permission issues wich means don’t have permission to read the current directory (rare)
