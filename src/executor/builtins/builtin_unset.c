/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moirhira <moirhira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 20:54:05 by ekhallaf          #+#    #+#             */
/*   Updated: 2025/08/07 20:32:07 by moirhira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

void    unset_variable(t_envp **env, char *var)
{
    t_envp *tmp = *env;
    t_envp *prev = NULL;
    
    while(tmp)
    {
        if(ft_strcmp(tmp->key, var) == 0)
        {
            if(prev)
                prev->next = tmp->next;
            else
                *env = tmp->next;
            return;    
        }
        prev = tmp;
        tmp = tmp->next;
    }
}

int builtin_unset(char **args, t_envp **env)
{
    int i = 1;

    while (args[i])
    {
        unset_variable(env, args[i]);
        i++;
    }
    return (0);
}
