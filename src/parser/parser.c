/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moirhira <moirhira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 21:07:57 by moirhira          #+#    #+#             */
/*   Updated: 2025/05/14 21:34:43 by moirhira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_command *parsing(t_token **token_lst, t_command **cmd_lst)
{
    t_command *head;
    t_token *token;
    
    head = NULL;      
    token = *token_lst;
    
    while (token)
    {
        if (!head)
        {
            add_command(cmd_lst, creat_command());
            head = *cmd_lst;
        }
        if((*token_lst)->type == 1)
        {
            printf("minishell: syntax error near unexpected token `%s'\n", token->value); 
            // free(head);
            return (NULL);
        }
        if (token->type == 1 && !handel_pipe(&token, &head, cmd_lst)) // | 
            return (NULL);
        else if (token->type == 2) // < 
        {
            if (!handel_input_redirection(&token, head))
                return (NULL);
        }
        else if (token->type == 3) // >
        {
           if (!handel_output_redirection(&token, head))
                return (NULL);
        }
        else if (token->type == 4) // >>
        {
            if (!handel_append_redirection(&token, head))
                return (NULL);
        }
        else if (token->type == 5) // << herdoc
        {
            if (!handel_heredoc(&token, head))
                return (NULL);
        }
        else
            handel_argument(&token, head);
    }
    return (t_command *)(1);
}
// print commands------------------------
void print_commands(t_command **commads);
//---------------------------------------
void parse_command(t_token **token_lst, t_command **command_lst, char *cmd_line, t_envp **my_env)
{
    split_token(cmd_line, my_env, token_lst);
    *command_lst = NULL;
    if (!parsing(token_lst, command_lst))
    {
        free_token(token_lst);
        free_env(my_env);
        free_command(command_lst);
        return;
    }
    print_commands(command_lst);
}







































void print_commands(t_command **commads)
{
    
    t_command *ptr;
    ptr = (*commads);
    while (ptr)
    {
        int i = 0;
        if (ptr->args)
        {
            while (ptr->args[i])
            {
                printf("argument num[%d]: %s\n", i + 1, ptr->args[i]);
                i++;
            }
        }
        // printf("is a pipe     : %d\n", ptr->pipe);
        printf("==============order==================\n");
        while (ptr->redirects)
        {
            printf("filename        : %s\n", ptr->redirects->filename);
            printf("type            : %d\n", ptr->redirects->type);
            ptr->redirects = ptr->redirects->next;
        }
        printf("=====================================\n");
        printf("....................................\n");
        ptr = ptr->next;
    }
}



/*
echo $$
# → syntax error (unless you implement PID)

echo $!
# → syntax error (unless you implement job control)

*/





// t_token *ptr;
//     ptr = *token_lst;
//     while (ptr)
//     {
//         printf("value       : %s\n", ptr->value);
//         printf("type        : %d\n", ptr->type);
//         printf("attached    : %d\n", ptr->attached);
//         printf("was quoted  : %d\n", ptr->was_quoted);
//         printf("....................................\n");
//         ptr = ptr->next;
//     }
    