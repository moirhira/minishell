/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekhallaf <ekhallaf@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 20:54:05 by ekhallaf          #+#    #+#             */
/*   Updated: 2025/08/02 21:48:20 by ekhallaf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

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
            free(tmp->key);
            free(tmp->value);
            free(tmp);
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
        if (is_valid_identifier(args[i]))
            unset_variable(env, args[i]);
        else
        {
            printf("unset: \'%s\': is not a valid identifier\n", args[i]);
            return 1;
        }
        i++;
    }
    return 0;
}
