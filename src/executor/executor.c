/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moirhira <moirhira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 21:07:51 by moirhira          #+#    #+#             */
/*   Updated: 2025/07/18 22:02:47 by moirhira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#include "../../include/minishell.h"
volatile sig_atomic_t g_signal_received = 0;




int execute_builtin()
{
    return (-1);
} 
// void setup_redirections(t_command *cmd)
// {
//     t_redirect *redir = cmd->redirects;
    
//     while (redir)
//     {
//         if (redir->type == TOKEN_HEREDOC || redir->type == TOKEN_HEREDOC_QUOTED)
//         {
//             int fd = open(redir->filename, O_RDONLY);
//             if (fd == -1)
//             {
//                 ft_putstr_fd("minishell: heredoc: Could not open temporary file\n", 2);
//                 exit(1);
//             }
//             dup2(fd, STDIN_FILENO);
//             close(fd);
//         }
//         redir = redir->next;
//     }
// }

// int execute_external(t_command *command, t_envp *env)
// {
//     int status;
//     int pid = fork();
//     if (pid == 0)
//     {
//         setup_redirections(command);
//         char *cat_args[] = {"cat", NULL};

//         execve("/bin/cat", cat_args, NULL);
//         perror("execve failed");
//         exit(EXIT_FAILURE);
//     }
//     else if (pid < 0)
//     {
//         perror("fork failed");
//         return (1);
//     }
//     else
//     {
//         waitpid(pid, &status, 0);
//         if (WIFEXITED(status))
//             return (WEXITSTATUS(status) + 128);
//         else if (WIFSIGNALED(status))
//             return (WTERMSIG(status));
//     }
//     return (0);
// }

int execute_external(t_command *command, t_envp *env)
{
    return (0);
}

int execute_commands(t_command *command, t_envp **my_env)
{
    int sts = 0;
    parse_heredocs(command, (*my_env));
    
    if (g_signal_received)
        return (exit_status(-1));
    
    int res = execute_builtin();
    if (res >= 0)
        return (exit_status(-1));
    sts = execute_external(command, (*my_env));
        
    return (exit_status(sts));
}