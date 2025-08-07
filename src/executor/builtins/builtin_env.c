/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moirhira <moirhira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 08:44:26 by ekhallaf          #+#    #+#             */
/*   Updated: 2025/08/07 15:08:53 by moirhira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

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



