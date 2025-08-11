/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cmd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moirhira <moirhira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 19:57:16 by moirhira          #+#    #+#             */
/*   Updated: 2025/08/11 21:00:59 by moirhira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"


static int save_std_in_out(t_command *cmd, int *s_stdin, int *s_stdout)
{
	*s_stdin = -1;
	*s_stdout = -1;
     
	if (has_input_redir(cmd))
	{
		*s_stdin = dup(STDIN_FILENO);
		if (*s_stdin == -1)
               return (display_error("dup Failed!", strerror(errno)), 1);
		fd_collector(*s_stdin, 0);
	}
	if (has_output_redir(cmd))
	{
		*s_stdout = dup(STDOUT_FILENO);
		if (*s_stdout == -1)
               return (display_error("dup Failed!", strerror(errno)), 1);
		fd_collector(*s_stdout, 0);
	}
	return (0);
}

static int     run_builtin(t_command *cmd, t_envp **env)
{
     int result;
     
	if (setup_redirections(cmd, 0) == 1)
          result = 1;
     else
     {
          if (ft_strcmp(cmd->args[0], "cd") == 0)
               result = builtin_cd(cmd, env);
          else if (ft_strcmp(cmd->args[0], "pwd") == 0)
               result = builtin_pwd(*env);
          else if (ft_strcmp(cmd->args[0], "echo") == 0)
               result = builtin_echo(cmd->args);
          else if (ft_strcmp(cmd->args[0], "env") == 0)
               result = builtin_env(*env);
          else if (ft_strcmp(cmd->args[0], "export") == 0)
               result = builtin_export(cmd,env);
          else if (ft_strcmp(cmd->args[0], "unset") == 0)
               result = builtin_unset(cmd->args, env);
          else if (ft_strcmp(cmd->args[0], "exit") == 0)
               result = builtin_exit(cmd->args);
          else
               result = -1;
     }
     return (result);
}


int execute_builtin(t_command *cmd, t_envp **env)
{
     int result;
     int s_stdin;
     int s_stdout;
     
     if (!is_builtin(cmd->args[0]))
          return(-1);
     if (save_std_in_out(cmd, &s_stdin, &s_stdout) == 1)
          return (-1);
     result = run_builtin(cmd, env);
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
     return (result);
}

