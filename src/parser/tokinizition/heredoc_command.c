/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_command.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moirhira <moirhira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 20:27:40 by moirhira          #+#    #+#             */
/*   Updated: 2025/08/09 20:59:41 by moirhira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../../../include/minishell.h"

int handel_heredoc_delimiter(char *s, int i, t_token **token, int *state)
{
	int 	k;
	int was_qoute;
	char    *delimiter_val;
	
    while (ft_isspace(s[i]))
		i++;
	delimiter_val = ft_calloc(ft_strlen(s) + 1, sizeof(char));
	if (!delimiter_val)
		return (-1);
	k = 0 ;
	was_qoute = 0;
    while (s[i] != '\0' && s[i] != ' ' && s[i] != '\t' 
        && s[i] != '|' && s[i] != '<' && s[i] != '>')
    {
        if (s[i] == '\'' || s[i] == '"')
        {
			char quote = s[i];
			was_qoute = 1;
			i++;
			while (s[i] && s[i] != quote)
			{
				delimiter_val[k++] = s[i++];
			}
			if (s[i] == quote)
				i++;
			else
			{
				ft_putstr_fd("minishell: Unclosed quote:", 2);
				ft_putstr_fd(&quote, 2);
				ft_putstr_fd("\n", 2);
				return (-1);
			}
        }
        else
            delimiter_val[k++] = s[i++];
    }
	add_token(token, create_token(delimiter_val, 0, 0, was_qoute));
	*state = 0;
    return (i);
}

