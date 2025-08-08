/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_redirections.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moirhira <moirhira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 06:48:59 by ekhallaf          #+#    #+#             */
/*   Updated: 2025/08/08 12:36:51 by moirhira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int f_error(int fd, t_redirect *redir, int target_fd)
{
    if (fd == -1)
    {
        display_error(redir->filename, strerror(errno));
        return 0;
    }
    
    if (dup2(fd, target_fd) == -1)
    {
        ft_putstr_fd("minishell: dup2 failed\n", 2);
        close(fd);
        return (0);
    }
    close (fd);
    return (1);
}

int setup_redirections(t_command *cmd, int exit_or_return)
{
    int fd;
    t_redirect *redir;
    
    redir = cmd->redirects;
    while (redir)
    {
        fd = -1;
        if (redir->type == TOKEN_HEREDOC_QUOTED ||
            redir->type == TOKEN_HEREDOC || redir->type == TOKEN_INPUT)
        {
            fd = open(redir->filename, O_RDONLY);
            if (!f_error(fd, redir, STDIN_FILENO))
            {
                if (exit_or_return)
                {
                    fd_collector(-1, 1);
                    free_all_memory();
                    exit(exit_status(1));
                }
                return(1);
            }
        }
        else if (redir->type == TOKEN_OUTPUT)
        {
            fd = open(redir->filename, O_WRONLY | O_CREAT |  O_TRUNC, 0644);
            if (!f_error(fd, redir, STDOUT_FILENO))
            {
                if (exit_or_return)
                {
                    fd_collector(-1, 1);
                    free_all_memory();
                    exit(exit_status(1));
                }
                return(1);
            }
        }
        else if (redir->type == TOKEN_APPEND)
        {
            fd = open(redir->filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
            if (!f_error(fd, redir, STDOUT_FILENO))
            {
                if (exit_or_return)
                {
                    fd_collector(-1, 1);
                    free_all_memory();
                    exit(exit_status(1));
                }
                return(1);
            }
        }
        redir = redir->next;
    }
    return (0);
}

// s_valid_identifier(const char *str)
// {
//     int i = 1;
//     if (!str || !str[0]) // check the first character of the arg 
//         return 0;
//     if (!ft_isalpha(str[0]) && str[0] != '_')
//         return 0;
//     while(str[i]) // check the rest of the arg
//     {
//         if (!ft_isalnum(str[i]) && str[i] != '_')
//             return 0;
//         i++;
//     }
//     return 1;
// }