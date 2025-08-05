/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cmd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moirhira <moirhira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 19:57:16 by moirhira          #+#    #+#             */
/*   Updated: 2025/08/05 19:57:22 by moirhira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int execute_builtin(t_command *cmd, t_envp **env)
{
     if (!is_builtin(cmd->args[0]))
          return(-1);
     int result;
     int s_stdin = -1;
    int s_stdout = -1;

    if (has_input_redir(cmd))
    {
        s_stdin = dup(STDIN_FILENO);
        if (s_stdin == -1)
        {
            display_error("dup Failed!", strerror(errno));
            close(s_stdin);
            return (1);
        }
    }
    

    if (has_output_redir(cmd))
    {
        s_stdout = dup(STDOUT_FILENO);
        if (s_stdout == -1)
        {
            display_error("dup Failed!", strerror(errno));
            close(s_stdout);
            return (1);
        }
    }
     
     if (setup_redirections(cmd, 0) == 1)
          return(1);
     if (strcmp(cmd->args[0], "cd") == 0)
          result = builtin_cd(cmd, env);
     else if (strcmp(cmd->args[0], "pwd") == 0)
          result = builtin_pwd(*env);
     else if (strcmp(cmd->args[0], "echo") == 0)
          result = builtin_echo(cmd->args);
     else if (strcmp(cmd->args[0], "env") == 0)
          result = builtin_env(*env);
     else if (strcmp(cmd->args[0], "export") == 0)
          result = builtin_export(cmd,env);
     else if (strcmp(cmd->args[0], "unset") == 0)
          result = builtin_unset(cmd->args, env);
     else if (strcmp(cmd->args[0], "exit") == 0)
          result = builtin_exit(cmd->args,env);
     else
          result = -1;
     //  fflush(stdout);

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
    return (result);
}

