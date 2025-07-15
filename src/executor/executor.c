/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moirhira <moirhira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 21:07:51 by moirhira          #+#    #+#             */
/*   Updated: 2025/07/15 15:55:21 by moirhira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../../include/minishell.h"


#include <string.h>
char *fill_herdoc(char *delimiter, int quoted)
{
    char *line;
    char *new_content = ft_calloc(1024, sizeof(char));
    
    while (1)
    {
        line = readline("> ");
        if (ft_strcmp(line, delimiter) == 0)
            break;
        else
            new_content = ft_strjoin(new_content, line);
    }
    return (new_content);
}

void parse_heredocs(t_command *command)
{
    t_command *cmd = command;
    while (cmd)
    {
        if (cmd->heredoc_count > 0)
        {
            t_redirect  *redir = cmd->redirects;
            while (redir)
            {
                if (redir->type == TOKEN_HEREDOC || redir->type == TOKEN_HEREDOC_QUOTED)
                {
                    char *delimiter = redir->filename;
                    int quoted = (redir->type == TOKEN_HEREDOC);
                    
                    char *content = fill_herdoc(delimiter, quoted);

                    if (content)
                        redir->content = content;
                }
                redir = redir->next;
            }
        }
        cmd = cmd->next;
    }
    
}


int execute_builtin()
{
    return (0);
} 

int execute_external()
{
    return (0);
}
int execute_commands(t_command *command)
{
    parse_heredocs(command);
    int res = execute_builtin();
    if (res >= 0)
    {
        //free
        return (res);
    }
    execute_external();
    return (0);
}