/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   single_command.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekhallaf <ekhallaf@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 06:49:13 by ekhallaf          #+#    #+#             */
/*   Updated: 2025/07/26 05:34:34 by ekhallaf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int    is_builtin(const char *cmd)
{
    const char *builtins[] = {"echo", "export", 
        "pwd", "cd", 
        "unset", "env",
        "exit"};
    
    int i = 0;
    while(builtins[i])
    {
        if(strcmp(cmd,builtins[i]) == 0)
            return 1;
    }
    return 0;
}

void free_env_array(char **envp)
{
    int i = 0;

    if (!envp)
        return;

    while (envp[i])
    {
        free(envp[i]);
        i++;
    }
    free(envp);
}

int exec_builtin_in_parent(t_command *cmd, t_envp **env)
{
    int status;
    if (setup_redirections(cmd) == -1)
        return 1;
    status = execute_builtin(cmd, env);
    return status;
}

int	execute_external(t_command *cmd, t_envp *env)
{
	pid_t	pid;
	char	*path;
	char	**envp;
	int		status;
    
	path = find_command_in_path(cmd->args[0], env);
	if (!path)
    {
		ft_putstr_fd("minishell: ", 2);
        ft_putstr_fd(cmd->args[0], 2);
        ft_putstr_fd(": command not found\n", 2);
        return (127);
    }
    
	envp = convert_env_to_array(env);
    if (!envp)
    {
        return (free(path), 1);
    }
	pid = fork();
	if (pid < 0)
	{
		perror("fork failed");
		free(path);
        free_array(envp, ft_strlen_2d(envp));
		return (1);
	}
	if (pid == 0)
	{
        setup_redirections(cmd);
		execve(path, cmd->args, envp);
        ft_putstr_fd("minishell: ", 2);
		perror(cmd->args[0]);
        
        free(path);
        free_array(envp, ft_strlen_2d(envp));
		exit(126);
	}
    else
    {
        free(path);
        free_array(envp, ft_strlen_2d(envp));
        waitpid(pid, &status, 0);
        
        if (WIFEXITED(status))
            return (WEXITSTATUS(status));
        else if (WIFSIGNALED(status))
            return (128 + WTERMSIG(status));
            
        return (0);
    }
}

