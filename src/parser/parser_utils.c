/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moirhira <moirhira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 20:48:08 by moirhira          #+#    #+#             */
/*   Updated: 2025/08/06 11:15:09 by moirhira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int handel_pipe(t_token **token, t_command **head, t_command **cmd_lst)
{
    add_command(cmd_lst, creat_command());
    (*head)->pipe = 1;
    *head = (*head)->next;
    *token = (*token)->next;
    return (1);
}
int handel_input_redirection(t_token **token, t_command *head)
{
    char *full_str;
    
    *token = (*token)->next;
    full_str = ft_strdup((*token)->value);
    if (!full_str)
        return (0);
    while ((*token)->attached)
    {
        *token = (*token)->next;
        full_str = ft_strjoin(full_str, (*token)->value);
    }
    head->infile_count++;
    add_redirect(head, 2, full_str);
    *token = (*token)->next;
    return (1);
}

int handel_output_redirection(t_token **token, t_command *head)
{
    char *full_str;
    
    *token = (*token)->next;
    full_str = ft_strdup((*token)->value);
    if (!full_str)
        return (0);
    while ((*token)->attached)
    {
        *token = (*token)->next;
        full_str = ft_strjoin(full_str, (*token)->value);
        // free(old);
    }
    head->outfile_count++;
    add_redirect(head, 3, full_str);
    *token = (*token)->next;
    // free(full_str);
    return (1);
}
int handel_append_redirection(t_token **token, t_command *head)
{
    char *full_str;
    
    *token = (*token)->next;
    full_str = ft_strdup((*token)->value);
    if (!full_str)
        return (0);
    while ((*token)->attached)
    {
        *token = (*token)->next;
        full_str = ft_strjoin(full_str, (*token)->value);
        // free(old);
    }
    head->append_count++;
    add_redirect(head, 4, full_str);
    *token = (*token)->next;
    // free(full_str);
    return (1);
}

int handel_heredoc(t_token **token, t_command *head)
{
    *token = (*token)->next;
    
    head->heredoc_count++;
    
    char *full_str = ft_strdup((*token)->value);
    if (!full_str)
        return (0);
        
    if ((*token)->was_quoted)
        add_redirect(head, 6, full_str);
    else
        add_redirect(head, 5, full_str);
        
    *token = (*token)->next;
    return (1);
}

void handel_argument(t_token **token, t_command *head)
{
    char *full_str = ft_strdup((*token)->value);
    if (!full_str)
        return;
    while ((*token)->attached)
    {
        *token = (*token)->next;
        full_str = ft_strjoin(full_str, (*token)->value);
        
        // free(old);
    }
    add_argument(head, full_str);
    // free(full_str);
    *token = (*token)->next;
}