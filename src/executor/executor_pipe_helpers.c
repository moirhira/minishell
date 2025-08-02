/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_pipe_helpers.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moirhira <moirhira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 05:17:32 by ekhallaf          #+#    #+#             */
/*   Updated: 2025/08/02 16:36:29 by moirhira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int exec_command(t_command *cmd, t_envp *env)
{
    char *path;
    char **envp;
    int status;

    path = find_command_in_path(cmd->args[0], env, &status);
	if (!path)
    {
        if (status == 126 || status == 127)
            return (status);
        else
        {
           	ft_putstr_fd("minishell: ", 2);
            ft_putstr_fd(cmd->args[0], 2);
            ft_putstr_fd(": command not found\n", 2);
            return (127);
        }
    }
    
    envp = convert_env_to_array(env);
    if (!envp)
    {
        return (free(path), 1);
    }
    execve(path, cmd->args, envp);
    ft_putstr_fd("minishell: ", 2);
	perror(cmd->args[0]);
    free(path);
    free_array(envp, ft_strlen_2d(envp));
    return(126);
}

void    child_process(t_command *cmd, int prev_pipe, int pipefd[2], t_envp *env)
{
    int exit_st = 0;
    
    if (prev_pipe != -1)
    {
        dup2(prev_pipe, STDIN_FILENO);
        close(prev_pipe);
    }
    if (cmd->pipe)
    {
        close(pipefd[0]);
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[1]);
    }
    setup_redirections(cmd, 1);
    if (is_builtin(cmd->args[0]))
        exit_st = execute_builtin(cmd, &env);
    else
        exit_st = exec_command(cmd, env);
    exit(exit_st);
}
