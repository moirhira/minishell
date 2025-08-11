/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   externals.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moirhira <moirhira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 06:49:13 by ekhallaf          #+#    #+#             */
/*   Updated: 2025/08/11 23:15:23 by moirhira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"


static int wait_for_child(pid_t pid)
{
    int status;
    
    waitpid(pid, &status, 0);
    setup_signals(SHELL_INTERACTIVE);
    if (WIFSIGNALED(status))
    {
        if (WTERMSIG(status) == SIGINT)
        {
            write(STDERR_FILENO,"\n", 1);
            return (130);
        }
        else if (WTERMSIG(status) == SIGQUIT)
        {
            write(STDERR_FILENO, "Quit (core dumped)\n", 20);
            return (131);
        }
    }
    if (WIFEXITED(status))
        return(exit_status(WEXITSTATUS(status)));
    return (0);
}

void process_child_external(t_command *cmd, char *path, char **envp)
{
    setup_signals(CHILD_PROCESS);
    if (setup_redirections(cmd, 1) == 1)
    {
        free_all_memory();
        fd_collector(-1, 1);
        exit(1);
    }
    execve(path, cmd->args, envp);
    display_error(cmd->args[0], strerror(errno));
    fd_collector(-1, 1);
    free_all_memory();
    exit(126);
}

int	execute_external(t_command *cmd, t_envp *env)
{
	pid_t	pid;
	char	*path;
	char	**envp;
	int		status;
    
    status = 0;
	path = find_command_in_path(cmd->args[0], env, &status);
	if (!path)
        return (status);
	envp = convert_env_to_array(env);
    setup_signals(SHELL_IGNORE);
	pid = fork();
	if (pid < 0)
        return(display_error("fork failed", strerror(errno)), 1);
	if (pid == 0)
	{
        process_child_external(cmd, path, envp);
	}
    return(wait_for_child(pid));
}

