/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moirhira <moirhira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 21:07:51 by moirhira          #+#    #+#             */
/*   Updated: 2025/07/17 23:01:03 by moirhira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#include "../../include/minishell.h"
volatile sig_atomic_t g_signal_received = 0;

void generate_unique_name(char temp_name[], size_t size_temp, pid_t pid)
{
    char *prefix = "/tmp/minishell_herdoc_";
    
    int i = 0;
    while(prefix[i] && i + 1 < size_temp)
    {
        temp_name[i] = prefix[i];
        i++;
    }
    char *char_pid = ft_itoa(pid);
    int j = 0;
    while(char_pid[j] && i + 1 < size_temp)
    {
        temp_name[i] = char_pid[j];
        i++;
        j++;
    }
    temp_name[i] = '\0';
}

int fill_herdoc(char *delimiter, int expnad_var, t_envp *my_env, char *temp_filename)
{
    pid_t pid;
    int status;
    
    setup_signals(SHELL_HEREDOC);
    pid = fork();
    if (pid == -1)
        return (0);
    
    if (pid == 0)
    {
        signal(SIGINT, SIG_DFL);
        signal(SIGQUIT, SIG_DFL);
        // setup_signals(SHELL_EXECUTING);
        int fd = open(temp_filename, O_WRONLY | O_CREAT | O_TRUNC, 0600);
        if (fd == -1)
            exit(EXIT_FAILURE);
            
        while (1)
        {
            char *line = readline("> ");
            
            if (!line)
            {
                ft_putstr_fd("warning: here-document delimited by end-of-file (wanted `", 2);
                ft_putstr_fd(delimiter, 2);
                ft_putstr_fd("')", 2);
                break;

            }
            
            if (ft_strcmp(line, delimiter) == 0)
            {
                free(line);
                break;
            }
            
            if (expnad_var)
            {
                char *processed_line = ft_calloc(1, sizeof(char));
                if (!processed_line)
                {
                    free(line);
                    close(fd);
                    exit(EXIT_FAILURE);
                }
                int i = 0;
                while (line[i])
                {
                    if (line[i] == '$')
                    {
                        processed_line = handel_env_var(line, &i, &my_env, processed_line);
                    }
                    else
                    {
                        char ch[2] = {line[i], '\0'};
                        char *old = processed_line;
                        processed_line = ft_strjoin(processed_line, ch);
                        free(old);
                        i++;
                    }
                }
                write(fd, processed_line, ft_strlen(processed_line));
                write(fd, "\n", 1);
                free(processed_line);
                free(line);
            }
            else
            {
                write(fd, line, ft_strlen(line));
                write(fd, "\n", 1);
                free(line);
            }
        }
        close(fd);
        exit(EXIT_SUCCESS);
    }
    else
    {
        waitpid(pid, &status, 0);
        
        setup_signals(SHELL_INTERACTIVE);
        
        if(WIFSIGNALED(status))
        {
            unlink(temp_filename);
            g_signal_received = 1;
            if (WTERMSIG(status) == SIGINT)
                exit_status(130);
            else if (WTERMSIG(status) == SIGQUIT)
                exit_status(131);
            return (0);
        }
        return (1);
    }
}

void parse_heredocs(t_command *command, t_envp *my_env)
{
    t_command *cmd = command;
    while (cmd && !g_signal_received)
    {
        if (cmd->heredoc_count > 0)
        {
            t_redirect  *redir = cmd->redirects;
            while (redir && !g_signal_received)
            {
                if (redir->type == TOKEN_HEREDOC || redir->type == TOKEN_HEREDOC_QUOTED)
                {
                    char *delimiter = redir->filename;
                    int expand_var = (redir->type == TOKEN_HEREDOC);
                    
                    char temp_filename[256];
                    generate_unique_name(temp_filename,sizeof(temp_filename), getpid());

                    if (fill_herdoc(delimiter, expand_var, my_env, temp_filename))
                        redir->filename = ft_strdup(temp_filename);
                    else if (g_signal_received)
                        break;
                }
                redir = redir->next;
            }
        }
        cmd = cmd->next;
    }
    
}


int execute_builtin()
{
    return (0);
} 

int execute_external()
{
    return (0);
}
int execute_commands(t_command *command, t_envp **my_env)
{
    parse_heredocs(command, (*my_env));
    int res = execute_builtin();
    if (res >= 0)
    {
        //free
        return (res);
    }
    execute_external();
    return (0);
}