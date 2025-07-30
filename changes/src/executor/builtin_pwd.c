/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_pwd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekhallaf <ekhallaf@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 08:44:41 by ekhallaf          #+#    #+#             */
/*   Updated: 2025/07/27 06:48:41 by ekhallaf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	builtin_pwd(t_envp *env)
{
	char *pwd;

	pwd = getenv("PWD");
	if (!pwd)
	{
		printf("pwd: PWD not set\n");
        return (exit_status(1));
	}
	printf("%s\n",pwd);
    return (exit_status(0));
}

//what i've changed

//old version :
///   uses getcwd() to query the kernel for the real current working directory.
///   this always reflects the real filesystem, even if PWD is wrong or unset.
///   if the directory was deleted, getcwd() fails, and it prints an error.

// second version (current):
///   uses getenv("PWD") to read the PWD environment variable.
///   this doesn't verify the filesystem—it just prints the value stored in the variable.
///   if the directory is deleted, it will still print the old value until i cd somewhere else.
