#include "../../include/minishell.h"


// int func_name
int execute_builtin(t_command *cmd, t_envp **env)
{
    if (strcmp(cmd->args[0], "cd") == 0)
        return (builtin_cd(cmd, env));
    else if (strcmp(cmd->args[0], "pwd") == 0)
         return (builtin_pwd(cmd));
    else if (strcmp(cmd->args[0], "echo") == 0)
         return (builtin_echo(cmd->args));
    else if (strcmp(cmd->args[0], "env") == 0)
         return (builtin_env(*env));
    else if (strcmp(cmd->args[0], "export") == 0)
         return (builtin_export(cmd,env));
     else if (strcmp(cmd->args[0], "unset") == 0)
          return (builtin_unset(cmd->args, env));
    else if (strcmp(cmd->args[0], "exit") == 0)
          return (builtin_exit(cmd->args,env));

    return (-1);
}

