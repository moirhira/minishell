/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   single_command.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekhallaf <ekhallaf@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 06:49:13 by ekhallaf          #+#    #+#             */
/*   Updated: 2025/07/15 21:11:14 by ekhallaf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char    *is_builtin(const char *cmd)
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


void execute_single_command(t_command *cmd, t_envp **env)
{
    setup_redirections(cmd);
    if(is_builtin(cmd->args[0]))
        execute_builtin(cmd, env);
    else
    {
        pid_t pid = fork();
        if (pid == 0)
        {
            char *path = find_command_in_path(cmd->args[0], env);
            execve(path, cmd->args, convert_env_to_array(*env));
            perror("minishell");
            exit(EXIT_FAILURE);
        }
        else if (pid > 0)
            waitpid(pid, &g_last_exit_status, 0);
        else
            perror("fork");
    }
    reset_redirections();
}
