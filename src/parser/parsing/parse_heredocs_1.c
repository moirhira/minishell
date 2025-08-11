/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_heredocs_1.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moirhira <moirhira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 09:39:57 by moirhira          #+#    #+#             */
/*   Updated: 2025/08/11 14:51:27 by moirhira         ###   ########.fr       */
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

static int process_herdoc(t_redirect *redir, t_envp *my_env)
{
    int expand_var;
    char *delimiter;
    char temp_filename[256];
    
    delimiter = redir->filename;
    expand_var = (redir->type == TOKEN_HEREDOC);
    generate_unique_name(temp_filename,sizeof(temp_filename), getpid());
    if (fill_herdoc(delimiter, expand_var, my_env, temp_filename))
    {
       redir->filename = ft_strdup(temp_filename);
       return (0);
    }
    else if (g_signal_received)
        return (1);
    return (0);
}

int parse_heredocs(t_command *command, t_envp *my_env)
{
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
                if (redir->type == TOKEN_HEREDOC 
                    || redir->type == TOKEN_HEREDOC_QUOTED)
                {
                    if (process_herdoc(redir, my_env))
                        return (1);
                }
                redir = redir->next;
            }
        }
        cmd = cmd->next;
    }
    return (0);
}
