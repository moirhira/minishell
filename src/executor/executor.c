/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moirhira <moirhira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 11:09:06 by ekhallaf          #+#    #+#             */
/*   Updated: 2025/08/03 17:14:40 by moirhira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"


int exec_redirs_no_command(t_command *command)
{
    int s_stdin = -1;
    int s_stdout = -1;

    if (has_input_redir(command))
    {
        s_stdin = dup(STDIN_FILENO);
        if (s_stdin == -1)
        {
            display_error("dup Failed!", strerror(errno));
            close(s_stdin);
            return (1);
        }
    }
    

    if (has_output_redir(command))
    {
        s_stdout = dup(STDOUT_FILENO);
        if (s_stdout == -1)
        {
            display_error("dup Failed!", strerror(errno));
            close(s_stdout);
            return (1);
        }
    }

    if (setup_redirections(command, 0) == 1)
    {
        if (s_stdin != -1)
        {
            if (dup2(s_stdin, STDIN_FILENO) == -1)
                display_error("dup2 Failed!", strerror(errno));
            close(s_stdin);
        }
        
        if (s_stdout != -1)
        {
            if (dup2(s_stdout, STDOUT_FILENO) == -1)
                display_error("dup2 Failed!", strerror(errno));
            close(s_stdout);
        }
        return (1);
    }
    
    if (s_stdin != -1)
    {
        if (dup2(s_stdin, STDIN_FILENO) == -1)
            display_error("dup2 Failed!", strerror(errno));
        close(s_stdin);
    }
    
    if (s_stdout != -1)
    {
        if (dup2(s_stdout, STDOUT_FILENO) == -1)
            display_error("dup2 Failed!", strerror(errno));
        close(s_stdout);
    }

    return (0);
}


int execute_commands(t_command *command, t_envp **env)
{
    int exit_st = 0;
    parse_heredocs(command, (*env));
   
    if (g_signal_received)
    {
        g_signal_received = 0;
        return (exit_status(130));
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
        {
            exit_st = exec_redirs_no_command(command);
            return (exit_status(exit_st));
        }
        return 0;
    }
    else if (is_builtin(command->args[0]))
    {
        exit_st = execute_builtin(command,env);
        return (exit_status(exit_st));
    }
    else
        exit_st = execute_external(command, *env);
        
    return (exit_status(exit_st));
}
    
    // if (!command->args || !command->args[0])
    // {
    //     if (command->redirects)
    //     {
    //         exit_st = exec_redirs_no_command(command);
    //         return (exit_status(exit_st));
    //     }
    //     return 0;
    // }
    
    // if (is_builtin(command->args[0]) && command->next == NULL)
    // {
    //     exit_st = execute_builtin(command,env);
    //     return (exit_status(exit_st));
    // }
    // else if (command && !command->pipe)
    // {
    //     exit_st = execute_external(command, *env);
    // }
    // else 
    // {
    //     exit_st = execute_pipeline(command, *env);
    // }