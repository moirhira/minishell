/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_redirections.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moirhira <moirhira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 06:48:59 by ekhallaf          #+#    #+#             */
/*   Updated: 2025/07/21 22:30:15 by moirhira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int f_error(int fd, t_redirect *redir, int target_fd)
{
    if (fd == -1)
    {
        ft_putstr_fd("minishell: ", 2);
        ft_putstr_fd(redir->filename, 2);
        ft_putstr_fd(": ", 2);
        perror("");
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
int setup_redirections(t_command *cmd)
{
    int fd;
    t_redirect *redir;
    
    redir = cmd->redirects;
    while (redir)
    {
        fd = -1;
        if (redir->type == TOKEN_HEREDOC_QUOTED || redir->type == TOKEN_HEREDOC)
        {
            fd = open(redir->filename, O_RDONLY);
            if (!f_error(fd, redir, STDIN_FILENO))
                return (0);
        }
        else if (redir->type == TOKEN_INPUT)
        {
            fd = open(redir->filename, O_RDONLY);
            if (!f_error(fd, redir, STDIN_FILENO))
                return (0);
        }
        else if (redir->type == TOKEN_OUTPUT)
        {
            fd = open(redir->filename,O_WRONLY | O_CREAT|  O_TRUNC, 0644);
            if (!f_error(fd, redir, STDOUT_FILENO))
                return(0);
        }
        else if (redir->type == TOKEN_APPEND)
        {
            fd = open(redir->filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
            if (!f_error(fd, redir, STDOUT_FILENO))
                return(0);
        }
        redir = redir->next;
    }
    
    return (1);
}

