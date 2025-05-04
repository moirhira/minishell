/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moirhira <moirhira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 20:48:08 by moirhira          #+#    #+#             */
/*   Updated: 2025/05/04 20:48:32 by moirhira         ###   ########.fr       */
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
    char **new_arr = realloc(head->infiles, sizeof(char *) * (head->infile_count + 2));
    if (!new_arr)
    {
        printf("error from realloc!\n");
        exit(1);
    }
    head->infiles = new_arr;
    head->infiles[head->infile_count] = ft_strdup((*token)->value);
    head->infile_count++;
    head->infiles[head->infile_count] = NULL; 
    *token = (*token)->next;
    return (1);
}

int handel_output_redirection(t_token **token, t_command *head)
{
    if (!check_next_token(*token, head))
        return (0);
    if ((*token)->type == 4)
        head->append = 1;
    if ((*token)->type == 3)
        head->append = 2;
    *token = (*token)->next;
    char **new_arr = realloc(head->outfiles, sizeof(char *) * (head->outfile_count + 2));
    if (!new_arr)
    {
        printf("error from realloc!\n");
        exit(1);
    }
    head->outfiles = new_arr;
    head->outfiles[head->outfile_count] = ft_strdup((*token)->value);
    head->outfile_count++;
    head->outfiles[head->outfile_count] = NULL; 
   *token = (*token)->next;
   return (1);
}

int handel_heredoc(t_token **token, t_command *head)
{
    if (!check_next_token(*token, head))
        return (0);
    *token = (*token)->next;
    char **new_arr = realloc(head->heredocs, sizeof(char *) * (head->heredoc_count + 2));
    if (!new_arr)
    {
        printf("error from realloc!\n");
        exit(1);
    }
    head->heredocs = new_arr;
    head->heredocs[head->heredoc_count] = ft_strdup((*token)->value);
    head->heredoc_count++;
    head->heredocs[head->heredoc_count] = NULL; 
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