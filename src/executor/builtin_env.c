/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekhallaf <ekhallaf@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 08:44:26 by ekhallaf          #+#    #+#             */
/*   Updated: 2025/08/02 21:46:40 by ekhallaf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../../include/minishell.h"

int builtin_env(t_envp *envp)
{
    if (!envp)
        return 1;
    t_envp *env = envp;
    
    while (env)
    {
        if (env->value)
            printf("%s=%s\n", env->key, env->value);
        env = env->next;
    }
    return 0;
}



