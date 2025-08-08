/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moirhira <moirhira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 11:09:06 by ekhallaf          #+#    #+#             */
/*   Updated: 2025/08/08 14:55:09 by moirhira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"


int exec_redirs_no_command(t_command *command)
{
    int s_stdin = -1;
    int s_stdout = -1;
    int status = 0;

    if (has_input_redir(command))
    {
        s_stdin = dup(STDIN_FILENO);
        fd_collector(s_stdin, 0);
        if (s_stdin == -1)
        {
            display_error("dup Failed!", strerror(errno));
            status = 1;
        }
    }
    

    if (has_output_redir(command))
    {
        s_stdout = dup(STDOUT_FILENO);
        fd_collector(s_stdout, 0);
        if (s_stdout == -1)
        {
            display_error("dup Failed!", strerror(errno));
            status = 1;
        }
    }

    if (setup_redirections(command, 0) == 1)
        status = 1;
    
    if (s_stdin != -1)
    {
        if (dup2(s_stdin, STDIN_FILENO) == -1)
            display_error("dup2 Failed!", strerror(errno));
    }
    if (s_stdout != -1)
    {
        if (dup2(s_stdout, STDOUT_FILENO) == -1)
            display_error("dup2 Failed!", strerror(errno));
    }
    fd_collector(-1, 1);
    return (status);
}


void cleanup_heredocs(t_command *command)
{
    t_command *current_cmd = command;
    t_redirect *redir;

    while (current_cmd)
    {
        redir = current_cmd->redirects;
        while (redir)
        {
            if (redir->type == TOKEN_HEREDOC || redir->type == TOKEN_HEREDOC_QUOTED)
            {
                if (redir->filename)
                {
                    unlink(redir->filename);
                    redir->filename = NULL;
                }
            }
            redir = redir->next;
        }
        current_cmd = current_cmd->next; 
    }
}

int execute_commands(t_command *command, t_envp **env)
{
    int exit_st = 0;
    parse_heredocs(command, (*env));
   
    if (g_signal_received)
    {
        g_signal_received = 0;
        exit_st = 130;
    }
    
    if (!command)
        return (0);
        

    if (command->pipe)
    {
        exit_st = execute_pipeline(command, *env);
    }
    else if (!command->pipe && (!command->args || !command->args[0]))
    {
        if (command->redirects)
            exit_st = exec_redirs_no_command(command);
    }
    else if (command && is_builtin(command->args[0]))
    {
        exit_st = execute_builtin(command,env);
    }
    else if (command)
    {
        exit_st = execute_external(command, *env);
    }
    cleanup_heredocs(command);
    return (exit_status(exit_st));
}
