/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   operator_command.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moirhira <moirhira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 15:33:44 by moirhira          #+#    #+#             */
/*   Updated: 2025/08/10 21:19:46 by moirhira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../../../include/minishell.h"

static int double_operator(char *s, int i, t_token **token, int *state)
{
    char symb_alloc[3];
    char *symb;
    
    symb_alloc[0] = s[i];
    symb_alloc[1] = s[i];
    symb_alloc[2] = '\0';
    symb = ft_strdup(symb_alloc);
    if (ft_strcmp(symb, ">>") == 0)
        add_token(token, create_token(symb, 4, 0, 0));
    else if (ft_strcmp(symb, "<<") == 0)
    {
        add_token(token, create_token(symb, 5, 0, 0));
        *state = 1;
    }
    return (i + 2);
}

static int single_operator(char *s, int i, t_token **token)
{
    char symb_alloc[2];
    char *symb;
    
    symb_alloc[0] = s[i];
    symb_alloc[1] = '\0';
    symb = ft_strdup(symb_alloc);
    if (ft_strcmp(symb, "|") == 0)
        add_token(token, create_token(symb, 1, 0, 0));
    else if (ft_strcmp(symb, "<") == 0)
        add_token(token, create_token(symb, 2, 0, 0));
    else if (ft_strcmp(symb, ">") == 0)
        add_token(token, create_token(symb, 3, 0, 0));
    return (i + 1);
}

int handel_operator(char *s, int i, t_token **token, int *state)
{
	if ((s[i] == '>' && s[i + 1] == '>') 
        || (s[i] == '<' && s[i + 1] == '<'))
	{
		return (double_operator(s, i, token, state));
	}
	else
	{
		return (single_operator(s, i, token));
	}
}

