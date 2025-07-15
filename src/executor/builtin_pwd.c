/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_pwd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekhallaf <ekhallaf@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 08:44:41 by ekhallaf          #+#    #+#             */
/*   Updated: 2025/07/15 21:04:20 by ekhallaf         ###   ########.fr       */
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
    return exit_status(0);
}

