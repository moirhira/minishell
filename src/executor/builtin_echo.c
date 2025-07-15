/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekhallaf <ekhallaf@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 06:48:34 by ekhallaf          #+#    #+#             */
/*   Updated: 2025/07/15 22:37:34 by ekhallaf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int     is_new_line(char *cmd)
{
    int i;
    if(ft_strcmp(cmd, "-n"))
        return 0;
    i = 2;
    while (cmd[i])
    {
        if(cmd[i] != 'n')
            return 0;
        i++;
    }
    return 1;
}

int    builtin_echo(char **cmd)
{
    int i = 1;
    int newline = 1;

    if(cmd[i] && is_new_line(cmd[i]))
    {
        newline = 0;
        i++;
    }
    while(cmd[i])
    {
        printf("%s", cmd[i]);
        if(cmd[i + 1])
            printf(" ");
        i++;
    }
    if (newline)
        printf("\n");
    
    return exit_status(0);
}
