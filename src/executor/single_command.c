/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   single_command.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moirhira <moirhira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 06:49:13 by ekhallaf          #+#    #+#             */
/*   Updated: 2025/08/03 17:13:11 by moirhira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int    is_builtin(const char *cmd)
{
    int i;
    
    const char *builtins[] = {
        "echo", "export", "pwd", "cd", 
        "unset", "env", "exit", NULL };
    
    i = 0;
    while(builtins[i])
    {
        if(ft_strcmp(cmd,builtins[i]) == 0)
            return (1);
        i++;    
    }
    return (0);
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



int	execute_external(t_command *cmd, t_envp *env)
{
	pid_t	pid;
	char	*path;
	char	**envp;
	int		status;
    
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
	pid = fork();
	if (pid < 0)
	{
        display_error("fork failed", strerror(errno));
		free(path);
        free_array(envp, ft_strlen_2d(envp));
		return (1);
	}
	if (pid == 0)
	{
        setup_signals(CHILD_PROCESS);
        setup_redirections(cmd, 1);
		execve(path, cmd->args, envp);
        display_error(cmd->args[0], strerror(errno));
        free(path);
        free_array(envp, ft_strlen_2d(envp));
		exit(126);
	}
    else
    {
        waitpid(pid, &status, 0);
        free(path);
        free_array(envp, ft_strlen_2d(envp));
        
        if (WIFSIGNALED(status))
        {
            if (WTERMSIG(status) == SIGINT)
                return (130);
            else if (WTERMSIG(status) == SIGQUIT)
            {
                write(STDOUT_FILENO, "Quit (core dumped)\n", 20);
                return (131);
            }
        }
        if (WIFEXITED(status))
           return(exit_status(WEXITSTATUS(status)));
        
        return (0);
    }
}

