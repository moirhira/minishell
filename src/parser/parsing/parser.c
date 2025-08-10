/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moirhira <moirhira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 21:07:57 by moirhira          #+#    #+#             */
/*   Updated: 2025/08/10 14:52:04 by moirhira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

int parsing_helper(t_token **token, t_command **head, t_command **cmd_lst)
{
    if ((*token)->type == 1)
        return (handel_pipe(token, head, cmd_lst));
    else if ((*token)->type == 2)
        return (handel_input_redirection(token, *head));
    else if ((*token)->type == 3)
        return (handel_output_redirection(token, *head));
    else if ((*token)->type == 4) 
        return (handel_append_redirection(token, *head));
    else if ((*token)->type == 5) 
        return (handel_heredoc(token, *head));
    else if ((*token)->ignored == 1)
    {
        exit_status(0);
        *token = (*token)->next;
    }
    else
    {
        return (handel_argument(token, *head));
    }
    return (1);
}

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
        if (!parsing_helper(&token, &head, cmd_lst))
            return (NULL);
    }
    return (head);
}

int parse_command(t_token **token_lst, t_command **command_lst, char *cmd_line, t_envp **my_env)
{
    if (!split_token(cmd_line, my_env, token_lst))
        return (exit_status(2), 2);

    if (!validate_syntaxe(token_lst))
    {
        exit_status (2);
        return (2);
    }
    *command_lst = NULL;
    if (!parsing(token_lst, command_lst))
    {
        return (exit_status(2), 2);
    }
    return (exit_status(-1), 0);
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
        t_redirect *ptr_red = ptr->redirects;
        while (ptr_red)
        {
            printf("filename        : %s\n", ptr_red->filename);
            printf("type            : %d\n", ptr_red->type);
            ptr_red = ptr_red->next;
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


// if (token->type == 1 || token->type == 2 || token->type == 3
//     || token->type == 4 || token->type == 5 || token->type == 6)
// {
//     next_token = token->next;
//     if (!next_token)
//     {
//         ft_putstr_fd("minishell: syntax error near unexpected token `newline'\n", 2);
//         return (0);
//     }
//     if (token->type == 1 && next_token->type == 1)
//     {
//         ft_putstr_fd("minishell: syntax error near unexpected token `||'\n", 2); 
//         return (0);
//     }
//     if (token->type != 1)
//     {
//         if (next_token->type != 0)
//         {
//             ft_putstr_fd("minishell: syntax error near unexpected token", 2 ); 
//             ft_putstr_fd(token->next->value, 2);
//             ft_putstr_fd("\n", 2);
//             return (0);
//         }
//     }
// }