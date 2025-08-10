/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_syntaxe.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moirhira <moirhira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 11:08:26 by moirhira          #+#    #+#             */
/*   Updated: 2025/08/10 21:20:18 by moirhira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../../../include/minishell.h"

int validate_helper(t_token *token)
{
    if (!token->next)
    {
        ft_putstr_fd("minishell: syntax error near unexpected token `newline'\n", 2);
        exit_status(2);
        return (0);
    }
    if (token->type == 1 && token->next->type == 1)
    {
        ft_putstr_fd("minishell: syntax error near unexpected token `||'\n", 2); 
        exit_status(2);
        return (0);
    }
    if (token->type != 1 && token->next->type != 0)
    {
        ft_putstr_fd("minishell: syntax error near unexpected token ", 2 ); 
        ft_putstr_fd(token->next->value, 2);
        ft_putstr_fd("\n", 2);
        exit_status(2);
        return (0);
    }
    return (1);
}


int validate_syntaxe(t_token **token_lst)
{
    t_token *token = *token_lst;
    
    if (!token)
        return (1);
    
    if (token->type == 1)
    {
        if (token->next && token->next->type == 1)
            printf("minishell: syntax error near unexpected token `||'\n");
        else
            printf("minishell: syntax error near unexpected token `|'\n");
        return (0);
    }
    while (token)
    {
        if (token->type != 0)
        {
           if (validate_helper(token) == 0)
                return (0);
        }
        token = token->next;
    }
    return (1);
}
