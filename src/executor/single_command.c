/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   single_command.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moirhira <moirhira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 06:49:13 by ekhallaf          #+#    #+#             */
/*   Updated: 2025/07/24 13:42:26 by moirhira         ###   ########.fr       */
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


static char **convert_env_to_array(t_envp *env)
{
    int count = 0;
    t_envp *tmp = env;
    
    while (tmp)
    {
        count++;
        tmp = tmp->next;
    }
    char **envp = malloc(sizeof(char *) * (count + 1));
    if (!envp)
        return NULL;
    
    tmp = env;
    int i = 0;
    while (tmp)
    {
        envp[i] = malloc(ft_strlen(tmp->key) + ft_strlen(tmp->value) + 2);
        if (!envp[i])
        {
            free_env_array(envp);
            return NULL;
        }
        
        strcpy(envp[i], tmp->key);
        strcat(envp[i], "=");
        strcat(envp[i], tmp->value);
        
        tmp = tmp->next;
        i++;
    }
    envp[count] = NULL;   
    return envp;
}


int	execute_external(t_command *cmd, t_envp *env)
{
	pid_t	pid;
	char	*path;
	char	**envp;
	int		status;
    // printf("command : %s\n", cmd->args[0]);
	path = find_command_in_path(cmd->args[0], env);
	if (!path)
    {
		ft_putstr_fd("minishell: ", 2);
        ft_putstr_fd(cmd->args[0], 2);
        ft_putstr_fd(": command not found\n", 2);
        return (127);
    }
	envp = convert_env_to_array(env);
	pid = fork();
	if (pid < 0)
	{
		perror("fork failed");
		free(path);
		return (0);
	}
	if (pid == 0)
	{
        setup_redirections(cmd);
		execve(path, cmd->args, envp);
		perror("minishell: ");
		exit(126);
	}
    else
    {
        free(path);
        waitpid(pid, &status, 0);
        return (1);
    }
    return (0);
}

