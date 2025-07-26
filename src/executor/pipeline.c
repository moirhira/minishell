/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekhallaf <ekhallaf@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 23:01:03 by ekhallaf          #+#    #+#             */
/*   Updated: 2025/07/26 05:35:39 by ekhallaf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int exec_command(t_command *cmd, t_envp *env)
{
    char *path;
    char **envp;

    path = find_command_in_path(cmd->args[0], env);
    if (!path)
    {
        ft_putstr_fd("minishell: ", 2);
        ft_putstr_fd(cmd->args[0], 2);
        ft_putstr_fd(": command not found\n", 2);
        return exit_status(127);
    }
    envp = convert_env_to_array(env);
    if (!envp)
    {
        free(path);
        return exit_status(1);
    }
    execve(path, cmd->args, envp);
    perror("execve failed");
    free(path);
    return exit_status(126);
}

static int wait_children(void)
{
    int status;
    int last_exit_status = 0;
    pid_t wpid;

    while ((wpid = wait(&status)) > 0)
    {
        if (WIFEXITED(status))
            last_exit_status = WEXITSTATUS(status);
        else if (WIFSIGNALED(status))
            last_exit_status = 128 + WTERMSIG(status);
    }
    exit_status(last_exit_status);
    return last_exit_status;
}

int handle_builtin_if_no_pipe(t_command *cmd, t_envp **env)
{
    if (is_builtin(cmd->args[0]) && cmd->pipe == 0)
    {
        int status = exec_builtin_in_parent(cmd, env);
        exit_status(status);
        return status;
    }
    return -1; 
}

int execute_pipeline(t_command *cmd_list, t_envp *env)
{
    int prev_pipe = -1;
    int pipefd[2];
    pid_t pid;
    t_command *cmd = cmd_list;

    while (cmd)
    {
        if (create_pipe_if_needed(cmd, pipefd) == -1)
            return 1;

        pid = fork();
        if (pid == -1)
        {
            perror("fork");
            exit_status(1);
            return 1;
        }
        if (pid == 0)
            child_process(cmd, prev_pipe, pipefd, env);

        prev_pipe = parent_pipe_cleanup(prev_pipe, cmd, pipefd);
        cmd = cmd->next;
    }
    return wait_children();
}

