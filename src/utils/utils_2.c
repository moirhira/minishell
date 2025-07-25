/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moirhira <moirhira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/19 15:51:44 by moirhira          #+#    #+#             */
/*   Updated: 2025/07/25 16:00:40 by moirhira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../../include/minishell.h"

int only_whitespace(char *str)
{
    int i;

    i = 0;
    while (str[i] != '\0')
    {
        if(!ft_isspace(str[i]))
            return (0);
        i++;
    }
    return (1);
}

char **convert_env_to_array(t_envp *env)
{
    int count = 0;
    t_envp *tmp = env;
    
    while (tmp)
    {
        count++;
        tmp = tmp->next;
    }
    char **envp = malloc(sizeof(char *) * (count + 1));
    if (!envp)
        return NULL;
    
    tmp = env;
    int i = 0;
    while (tmp)
    {
        envp[i] = malloc(ft_strlen(tmp->key) + ft_strlen(tmp->value) + 2);
        if (!envp[i])
        {
            free_env_array(envp);
            return NULL;
        }
        
        strcpy(envp[i], tmp->key);
        strcat(envp[i], "=");
        strcat(envp[i], tmp->value);
        
        tmp = tmp->next;
        i++;
    }
    envp[count] = NULL;   
    return envp;
}