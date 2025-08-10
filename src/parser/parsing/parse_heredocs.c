/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_heredocs.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moirhira <moirhira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 09:39:57 by moirhira          #+#    #+#             */
/*   Updated: 2025/08/10 22:44:04 by moirhira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

void generate_unique_name(char temp_name[], size_t size_temp, pid_t pid)
{
    char *char_pid;
    char *prefix;

    prefix = "/tmp/minishell_herdoc_";
    int i = 0;
    while(prefix[i] && i + 1 < size_temp)
    {
        temp_name[i] = prefix[i];
        i++;
    }
    char_pid = ft_itoa(pid);
    int j = 0;
    while(char_pid[j] && i + 1 < size_temp)
    {
        temp_name[i] = char_pid[j];
        i++;
        j++;
    }
    temp_name[i] = '\0';
}

void read_from_heredoc(int fd, char *line, t_envp *my_env, int expnad_var)
{
    char *processed_line;
    int i;

    if (expnad_var)
    {
        processed_line = ft_calloc(1, sizeof(char));
        i = 0;
        while (line[i])
        {
            if (line[i] == '$')
                processed_line = handel_env_var(line, &i, &my_env, processed_line);
            else
            {
                char ch[2] = {line[i], '\0'};
                processed_line = ft_strjoin(processed_line, ch);
                i++;
            }
        }
        write(fd, processed_line, ft_strlen(processed_line));
        write(fd, "\n", 1);
    }
    else
    {
        write(fd, line, ft_strlen(line));
        write(fd, "\n", 1);
    }
}

void proccess_child(char *delimiter, int expnad, t_envp *env, char *filename)
{
    int fd;
    
    setup_signals(SHELL_HEREDOC);
    fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0600);
    if (fd == -1)
        exit(EXIT_FAILURE);
    fd_collector(fd, 0);
    while (1)
    {
        char *line = readline("> ");
        if (!line)
        {
            ft_putstr_fd("warning: here-document delimited by end-of-file (wanted `", 2);
            ft_putstr_fd(delimiter, 2);
            ft_putstr_fd("')\n", 2);
            break;
        }
        if (ft_strcmp(line, delimiter) == 0)
        {   
            free(line);
            break;
        }
        read_from_heredoc(fd, line, env, expnad);
        free(line);
    }
    close(fd);
    free_all_memory();
    exit(EXIT_SUCCESS);
}

int fill_herdoc(char *delimiter, int expnad_var, t_envp *my_env, char *temp_filename)
{
    pid_t pid;
    int status;
    
    setup_signals(SHELL_IGNORE);
    pid = fork();
    if (pid == -1)
        return (0);
    if (pid == 0)
        proccess_child(delimiter, expnad_var, my_env, temp_filename);
    else
    {
        waitpid(pid, &status, 0);
        setup_signals(SHELL_INTERACTIVE);
        if(WIFEXITED(status))
        {
            if (WEXITSTATUS(status) == EXIT_SUCCESS)
                return (1);
            else if (WEXITSTATUS(status) == 130)
            {
                g_signal_received = SIGINT;
                unlink(temp_filename);
                return (0);
            }
            else
            {
                g_signal_received = WEXITSTATUS(status);
                unlink (temp_filename);
                return (0);
            }
        }
        return (1);
    }
    return (0);
}

int parse_heredocs(t_command *command, t_envp *my_env)
{
    int expand_var;
    char *delimiter;
    char temp_filename[256];
    t_redirect  *redir;
    t_command *cmd;
    
    cmd = command;
    g_signal_received = 0;
    while (cmd)
    {
        if (cmd->heredoc_count > 0)
        {
            redir = cmd->redirects;
            while (redir)
            {
                if (redir->type == TOKEN_HEREDOC || redir->type == TOKEN_HEREDOC_QUOTED)
                {
                    delimiter = redir->filename;
                    expand_var = (redir->type == TOKEN_HEREDOC);
                    generate_unique_name(temp_filename,sizeof(temp_filename), getpid());
                    if (fill_herdoc(delimiter, expand_var, my_env, temp_filename))
                        redir->filename = ft_strdup(temp_filename);
                    else if (g_signal_received)
                        return (1);
                }
                redir = redir->next;
            }
        }
        cmd = cmd->next;
    }
    return (0);
}
