/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_pwd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moirhira <moirhira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 08:44:41 by ekhallaf          #+#    #+#             */
/*   Updated: 2025/07/24 21:00:30 by moirhira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int    builtin_pwd(void)
{
    char cwd[PATH_MAX]; 
    if(getcwd(cwd, sizeof(cwd)) == NULL)
    {
        perror("pwd");
        return exit_status(1);
    }
    printf("%s\n", cwd);
    return (exit_status(-1));
}

