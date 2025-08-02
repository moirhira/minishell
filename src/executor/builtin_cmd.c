#include "../../include/minishell.h"



int has_input_redir(t_command *cmd)
{
    t_redirect *redir;

    if (!cmd || !cmd->redirects)
        return (0);

    redir = cmd->redirects;
    while (redir)
    {
        if (redir->type == TOKEN_INPUT || redir->type == TOKEN_HEREDOC)
            return (1); // Found one, no need to look further
        redir = redir->next;
    }
    return (0); // No input redirections found
}

int has_output_redir(t_command *cmd)
{
    t_redirect *redir;

    if (!cmd || !cmd->redirects)
        return (0);

    redir = cmd->redirects;
    while (redir)
    {
        if (redir->type == TOKEN_OUTPUT || redir->type == TOKEN_APPEND)
            return (1); // Found one, no need to look further
        redir = redir->next;
    }
    return (0); // No output redirections found
}

int execute_builtin(t_command *cmd, t_envp **env)
{
     if (!is_builtin(cmd->args[0]))
          return(-1);
     int result;
     int s_stdin = -1;
     int s_stdout = -1;

     if (has_input_redir(cmd))
          s_stdin = dup(STDIN_FILENO);

     if (has_output_redir(cmd))
          s_stdout = dup(STDOUT_FILENO);
     
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
          dup2(s_stdin, STDIN_FILENO);
          close(s_stdin);
     }
     if (s_stdout != -1)
     {
          dup2(s_stdout, STDOUT_FILENO);
          close(s_stdout);
     }
    return (result);
}

