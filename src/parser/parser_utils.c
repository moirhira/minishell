/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moirhira <moirhira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 20:48:08 by moirhira          #+#    #+#             */
/*   Updated: 2025/07/27 17:57:10 by moirhira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int handel_pipe(t_token **token, t_command **head, t_command **cmd_lst)
{
    if (!check_next_token(*token, *head))
        return (0);
    add_command(cmd_lst, creat_command());
    (*head)->pipe = 1;
    *head = (*head)->next;
    *token = (*token)->next;
    return (1);
}
int handel_input_redirection(t_token **token, t_command *head)
{
    if (!check_next_token(*token, head))
        return (0);
    *token = (*token)->next;
    head->infile_count++;
    add_redirect(head, 2, (*token)->value);
    *token = (*token)->next;
    return (1);
}

int handel_output_redirection(t_token **token, t_command *head)
{
    if (!check_next_token(*token, head))
        return (0);
    *token = (*token)->next;
    head->outfile_count++;
    add_redirect(head, 3, (*token)->value);
    *token = (*token)->next;
    return (1);
}
int handel_append_redirection(t_token **token, t_command *head)
{
    if (!check_next_token(*token, head))
        return (0);
    *token = (*token)->next;
    head->append_count++;
    add_redirect(head, 4, (*token)->value);
    *token = (*token)->next;
    return (1);
}

int handel_heredoc(t_token **token, t_command *head)
{
    int need_quote = 0;
    if (!check_next_token(*token, head))
        return (0);
    *token = (*token)->next;
    head->heredoc_count++;
    //----------------------------------------------
    char *full_str = ft_strdup((*token)->value);
    if (!full_str)
        return (0);
    while ((*token)->attached)
    {
        if ((*token)->was_quoted)
            need_quote = 1;
        char *old = full_str;
        *token = (*token)->next;
        full_str = ft_strjoin(full_str, (*token)->value);
        free(old);
    }
    //----------------------------------------------
    if (need_quote || (*token)->was_quoted)
        add_redirect(head, 6, full_str);
    else
        add_redirect(head, 5, full_str);
    *token = (*token)->next;
    free(full_str);
    return (1);
}

void handel_argument(t_token **token, t_command *head)
{
    char *full_str = ft_strdup((*token)->value);
    if (!full_str)
        return;
    while ((*token)->attached)
    {
        char *old = full_str;
        *token = (*token)->next;
        full_str = ft_strjoin(full_str, (*token)->value);
        free(old);
    }
    add_argument(head, full_str);
    free(full_str);
    *token = (*token)->next;
}