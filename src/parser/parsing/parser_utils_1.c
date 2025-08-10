/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils_1.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moirhira <moirhira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 20:55:09 by moirhira          #+#    #+#             */
/*   Updated: 2025/08/10 15:07:34 by moirhira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

t_command *creat_command(void)
{
    t_command *new_cmd;
    new_cmd = (t_command *)ft_malloc(sizeof(t_command));
    if (!new_cmd)
        return (NULL);
    new_cmd->args = NULL;
    new_cmd->heredoc_count = 0;
    new_cmd->pipe = 0;
    new_cmd->redirects = NULL;
    new_cmd->next = NULL;
    return (new_cmd);
}

void add_command(t_command **command_lst, t_command *new_command)
{
    t_command *ptr;
    if (!command_lst || !new_command)
        return;
    if (!*command_lst)
    {
        *command_lst = new_command;
        return;
    }
    ptr = *command_lst;
    while (ptr->next)
        ptr = ptr->next;
    ptr->next = new_command;    
}

void  add_redirect(t_command *cmd, int type, const char *filename)
{
    t_redirect *new, *ptr;
    
    ptr = NULL;
    new = (t_redirect *)ft_malloc(sizeof(t_redirect));
    if (!new)
        return;
    new->filename = ft_strdup(filename);
    new->type = type;
    new->content = NULL;
    new->next = NULL;
    if (!cmd->redirects)
        cmd->redirects = new;
    else
    {
        ptr = cmd->redirects;
        while (ptr->next)
            ptr = ptr->next;
        ptr->next = new;
    }   
}

int init_args(t_command *cmd, char *arg)
{
    cmd->args = (char **)ft_malloc(sizeof(char *) * 2);
    if (!cmd->args)
        return (0);
    cmd->args[0] = ft_strdup(arg);
    cmd->args[1] = NULL ;
    return (1);
}

int extend_args(t_command *cmd, char *arg)
{
    int len;
    int i;
    char **new_args;
    
    len = 0;
    while (cmd->args[len])
        len++;
    new_args = (char **)ft_malloc(sizeof(char *) * (len + 2));
    if (!new_args)
        return (0);
    i = 0;
    while (i < len)
    {
        new_args[i] = cmd->args[i];
        i++;
    }
    new_args[i++] = ft_strdup(arg);
    new_args[i] = NULL;
    cmd->args = new_args;
    return (1);
}

int add_argument(t_command *cmd, char *arg)
{
    if (!arg)
        return (0);
    if (!cmd->args)
        return (init_args(cmd, arg));
    else
        return (extend_args(cmd, arg));
}

