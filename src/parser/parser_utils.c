/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moirhira <moirhira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 20:48:08 by moirhira          #+#    #+#             */
/*   Updated: 2025/05/11 20:48:35 by moirhira         ###   ########.fr       */
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
    if (!check_next_token(*token, head))
        return (0);
    *token = (*token)->next;
    head->heredoc_count++;
    if ((*token)->was_quoted)
        add_redirect(head, 6, (*token)->value);
    else
        add_redirect(head, 5, (*token)->value);
    *token = (*token)->next;
    return (1);
}

void handel_argument(t_token **token, t_command *head)
{
    char *full_str = ft_strdup((*token)->value);
    while ((*token)->attached)
    {
        *token = (*token)->next;
        full_str = ft_strjoin(full_str, (*token)->value);
    }
    add_argument(head, full_str);
    free(full_str);
    *token = (*token)->next;
}