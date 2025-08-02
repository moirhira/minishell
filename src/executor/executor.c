/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekhallaf <ekhallaf@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 11:09:06 by ekhallaf          #+#    #+#             */
/*   Updated: 2025/08/02 21:48:37 by ekhallaf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int exec_redirs_no_command(t_command *command)
{
    int s_stdin = -1;
    int s_stdout = -1;

    if (command->redirects)
    {
        s_stdin = dup(STDIN_FILENO);
        s_stdout = dup(STDOUT_FILENO);
        if (s_stdin == -1 || s_stdout == -1)
        {
            perror("dup failed");
            close(s_stdin);
            close(s_stdout);
            return (1);
        }
    }
    if (setup_redirections(command, 0) == 1)
        return (1);
    if (command->redirects)
    {
        if (dup2(s_stdin, STDIN_FILENO) == -1 || dup2(s_stdout, STDOUT_FILENO) == -1)
            perror("failed to restore file descriptor");
        close(s_stdin);
        close(s_stdout);
    }
    return (0);
}

static int	handle_heredoc_signal(t_command *cmd, t_envp **env)
{
    parse_heredocs(cmd, *env);
	if (g_signal_received)
	{
        write(STDOUT_FILENO, "\n", 1);
		g_signal_received = 0;
		return (exit_status(130));
	}
	return (-1);
}
static int	handle_no_args_command(t_command *cmd)
{
    if (cmd->redirects)
    {
        return (exit_status(exec_redirs_no_command(cmd)));
    }
	return (0);
}
static int	execute_single_command(t_command *cmd, t_envp **env)
{
    int	status;
    
	if (!cmd->args || !cmd->args[0])
        return (handle_no_args_command(cmd));
	status = execute_builtin(cmd, env);
	if (status >= 0)
        return (exit_status(status));
	status = execute_external(cmd, *env);
	return (exit_status(status));
}
int	execute_commands(t_command *cmd, t_envp **env)
{
    int	status;
    
	status = handle_heredoc_signal(cmd, env);
	if (status != -1)
        return (status);
	if (cmd && cmd->next == NULL)
		return (execute_single_command(cmd, env));
    if (cmd && cmd->pipe)
		return (exit_status(execute_pipeline(cmd, *env)));
    return (0);
}



// int execute_commands(t_command *command, t_envp **env)
// {
//     int exit_st = 0;
//     parse_heredocs(command, (*env));
   
//     if (g_signal_received)
//     {
//         write(STDOUT_FILENO, "\n", 1);
//         g_signal_received = 0;
//         return (exit_status(130));
//     }
//     if (command && command->next == NULL)
//     {
//         if (!command->args || !command->args[0])
//         {
//             if (command->redirects)
//             {
//                 exit_st = exec_redirs_no_command(command);
//                 return (exit_status(exit_st));
//             }
//             return 0;
//         }
//         exit_st = execute_builtin(command,env);
//         if (exit_st >= 0)
//             return (exit_status(exit_st));
//         exit_st = execute_external(command, *env);
//         return (exit_status(exit_st));
//     }
//     else if (command && command->pipe)
//     {
//         exit_st = execute_pipeline(command, *env);
//         return (exit_status(exit_st));
//     }
//     return (0);
// }
