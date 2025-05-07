/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils_1.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moirhira <moirhira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 20:55:09 by moirhira          #+#    #+#             */
/*   Updated: 2025/05/07 11:26:03 by moirhira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_command *creat_command(void)
{
    t_command *new_cmd;
    new_cmd = (t_command *)malloc(sizeof(t_command));
    if (!new_cmd)
        return (NULL);
    new_cmd->args = NULL;
    new_cmd->heredocs = NULL;
    new_cmd->heredoc_count = 0;
    new_cmd->infiles = NULL;
    new_cmd->infile_count = 0;
    new_cmd->outfiles = NULL;
    new_cmd->outfile_count = 0;
    new_cmd->appends = NULL;
    new_cmd->append_count = 0;
    new_cmd->pipe = 0;
    new_cmd->infos = NULL;
    new_cmd->next = NULL;
    return (new_cmd);
}

t_redirect *create_info_command()
{
    t_redirect *new_info;
    new_info = malloc(sizeof(t_redirect));
    if (!new_info)
        return (NULL);
    new_info->filename = NULL;
    new_info->type = 0;
    return (new_info);
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

    


void add_argument(t_command *cmd, char *arg)
{
    int len;
    char **new_args;
    len = 0;
    if (!arg)
        return;
    if (!cmd->args)
    {
        cmd->args = (char **)malloc(sizeof(char *) * 2);
        if (!cmd->args)
            return;
        cmd->args[0] = ft_strdup(arg);
        cmd->args[1] = NULL ;
        return;
    }
    while (cmd->args[len])
        len++;
    new_args = (char **)malloc(sizeof(char *) * (len + 2));
    if (!new_args)
        return;
    int i = 0;
    while (i < len)
    {
        new_args[i] = cmd->args[i];
        i++;
    }
    new_args[i++] = ft_strdup(arg);
    new_args[i] = NULL;
    free(cmd->args);
    cmd->args = new_args;
}

int check_next_token(t_token *token, t_command *head)
{
    if (!token->next)
    {
        printf("minishell: syntax error near unexpected token `newline'\n");
        free(head);
        return (0);
    }
    if (token->next->type != 0)
    {
        printf("minishell: syntax error near unexpected token `%s'\n", token->next->value); 
        free(head);
        return (0);
    }
    return (1);
}