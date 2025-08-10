/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moirhira <moirhira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 22:07:56 by moirhira          #+#    #+#             */
/*   Updated: 2025/08/09 21:03:23 by moirhira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"


int exit_status(int new_status)
{
    static int status = 0;

    if (new_status != -1)
        status = new_status;
    return (status);
}

void fd_collector(int fd, int mode)
{
    static int fds[1024];
    static int count;
    int i;
    
    if (mode == 0)
    {
        if (fd >= 0 && count < 1024)
            fds[count++] = fd;
    }
    else if (mode == 1)
    {
        i = 0;
        while (i < count)
            close(fds[i++]);
        count = 0;
    }
}

