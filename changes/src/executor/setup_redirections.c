/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_redirections.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekhallaf <ekhallaf@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 06:48:59 by ekhallaf          #+#    #+#             */
/*   Updated: 2025/07/27 22:48:39 by ekhallaf         ###   ########.fr       */
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
static int open_and_check(t_redirect *redir, int flags, mode_t mode, int std_fd)
{
    int fd;

    if (mode)
        fd = open(redir->filename, flags, mode);
    else
        fd = open(redir->filename, flags);
    if (!f_error(fd, redir, std_fd))
        return 0;
    return 1;
}

int setup_redirections(t_command *cmd)
{
    t_redirect *redir = cmd->redirects;

    while (redir)
    {
        if (redir->type == TOKEN_HEREDOC || redir->type == TOKEN_HEREDOC_QUOTED || redir->type == TOKEN_INPUT)
        {
            if (!open_and_check(redir, O_RDONLY, 0, STDIN_FILENO))
                return 0;
        }
        else if (redir->type == TOKEN_OUTPUT)
        {
            if (!open_and_check(redir, O_WRONLY | O_CREAT | O_TRUNC, 0644, STDOUT_FILENO))
                return 0;
        }
        else if (redir->type == TOKEN_APPEND)
        {
            if (!open_and_check(redir, O_WRONLY | O_CREAT | O_APPEND, 0644, STDOUT_FILENO))
                return 0;
        }
        redir = redir->next;
    }
    return 1;
}


