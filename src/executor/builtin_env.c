/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekhallaf <ekhallaf@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 08:44:26 by ekhallaf          #+#    #+#             */
/*   Updated: 2025/07/16 00:04:09 by ekhallaf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../../include/minishell.h"

int builtin_env(t_envp *envp)
{
    if (!envp)
        return exit_status(1);
    t_envp *env = envp;
    
    while (env)
    {
        if (env->value)
            printf("%s=%s\n", env->key, env->value);
        env = env->next;
    }
    return exit_status(0);
}



