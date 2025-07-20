/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   single_command.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekhallaf <ekhallaf@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 06:49:13 by ekhallaf          #+#    #+#             */
/*   Updated: 2025/07/20 02:16:17 by ekhallaf         ###   ########.fr       */
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
    
    // Count variables with while
    while (tmp)
    {
        count++;
        tmp = tmp->next;
    }
    // Allocate array (+1 for NULL terminator)
    char **envp = malloc(sizeof(char *) * (count + 1));
    if (!envp)
        return NULL;
    
    // Fill array with while
    tmp = env;
    int i = 0;
    while (tmp)
    {
        // Allocate space for "KEY=VALUE\0"
        envp[i] = malloc(ft_strlen(tmp->key) + ft_strlen(tmp->value) + 2);
        if (!envp[i])
        {
            free_env_array(envp);
            return NULL;
        }
        
        // Build string manually
        strcpy(envp[i], tmp->key);      // Copy key
        strcat(envp[i], "=");           // Add equals
        strcat(envp[i], tmp->value);    // Add value
        
        tmp = tmp->next;
        i++;
    }
    envp[count] = NULL;   
    return envp;
}

static void reset_redirections(void)
{
    dup2(STDIN_FILENO, STDIN_FILENO);
    dup2(STDOUT_FILENO, STDOUT_FILENO);
}


void	execute_single_command(t_command *cmd, t_envp **env)
{
	pid_t	pid;
	char	*path;
	char	**envp;
	int		status;

	path = find_command_in_path(cmd->args[0], *env);
	if (!path)
		return ((void)printf("%s: command not found\n", cmd->args[0]));
	envp = convert_env_to_array(*env);
	pid = fork();
	if (pid < 0)
	{
		perror("fork failed");
		free(path);
		return ;
	}
	if (pid == 0)
	{
		execve(path, cmd->args, envp);
		perror("execve failed");
		exit(EXIT_FAILURE);
	}
	waitpid(pid, &status, 0);
	free(path);
}

