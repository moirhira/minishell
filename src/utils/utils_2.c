/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moirhira <moirhira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/19 15:51:44 by moirhira          #+#    #+#             */
/*   Updated: 2025/08/04 22:29:14 by moirhira         ###   ########.fr       */
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
    char **envp = ft_malloc(sizeof(char *) * (count + 1));
    if (!envp)
        return NULL;
    
    tmp = env;
    int i = 0;
    while (tmp)
    {
        envp[i] = ft_malloc(ft_strlen(tmp->key) + ft_strlen(tmp->value) + 2);
        if (!envp[i])
        {
            // free_env_array(envp);
            return NULL;
        }
        
        ft_strcpy(envp[i], tmp->key);
        ft_strcat(envp[i], "=");
        ft_strcat(envp[i], tmp->value);
        
        tmp = tmp->next;
        i++;
    }
    envp[count] = NULL;   
    return envp;
}
int	ft_lstsize(t_envp *lst)
{
	int	i;

	if (!lst)
		return (0);
	i = 0;
	while (lst != NULL)
	{
		lst = lst->next;
		i++;
	}
	return (i);
}

int has_input_redir(t_command *cmd)
{
    t_redirect *redir;

    if (!cmd || !cmd->redirects)
        return (0);

    redir = cmd->redirects;
    while (redir)
    {
        if (redir->type == TOKEN_INPUT || redir->type == TOKEN_HEREDOC || redir->type == TOKEN_HEREDOC_QUOTED)
            return (1);
        redir = redir->next;
    }
    return (0);
}

int has_output_redir(t_command *cmd)
{
    t_redirect *redir;

    if (!cmd || !cmd->redirects)
        return (0);

    redir = cmd->redirects;
    while (redir)
    {
        if (redir->type == TOKEN_OUTPUT || redir->type == TOKEN_APPEND)
            return (1);
        redir = redir->next;
    }
    return (0);
}