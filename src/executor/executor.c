/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moirhira <moirhira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 21:07:51 by moirhira          #+#    #+#             */
/*   Updated: 2025/07/16 22:50:34 by moirhira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../../include/minishell.h"


#include <string.h>
char *fill_herdoc(char *delimiter, int expnad_var, t_envp *my_env)
{
    char *line;
    char *new_content = ft_calloc(1, sizeof(char));
    if (!new_content)
        return (NULL);
    
    while (1)
    {
        line = readline("> ");
        
        if (!line)
        {
            ft_putstr_fd("warning: here-document delimited by end-of-file (wanted `", 2);
            ft_putstr_fd(delimiter, 2);
            ft_putstr_fd("')", 2);
            break;

        }
        
        if (ft_strcmp(line, delimiter) == 0)
        {
            free(line);
            break;
        }
        
        char *processed_line;
        if (expnad_var)
        {
            processed_line = ft_calloc(1, sizeof(char));
            if (!processed_line)
            {
                free(line);
                free(new_content);
                return (NULL);
            }
            int i = 0;
            while (line[i])
            {
                if (line[i] == '$')
                {
                    processed_line = handel_env_var(line, &i, &my_env, processed_line);
                }
                else
                {
                    char ch[2] = {line[i], '\0'};
                    char *old = processed_line;
                    processed_line = ft_strjoin(processed_line, ch);
                    free(old);
                    i++;
                }
            }
            free(line);
        }
        else
            processed_line = line;
        
        char *old = new_content;
        new_content = ft_strjoin(new_content, processed_line);
        free(old);

        old = new_content;
        new_content = ft_strjoin(new_content, "\n");
        free(old);
        free(processed_line);
    }
    return (new_content);
}

void parse_heredocs(t_command *command, t_envp *my_env)
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
                    int expand_var = (redir->type == TOKEN_HEREDOC);
                    
                    char *content = fill_herdoc(delimiter, expand_var, my_env);

                    if (content)
                        redir->content = content;
                    else
                        ft_strdup("");
                    printf("%s", redir->content);
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
int execute_commands(t_command *command, t_envp **my_env)
{
    parse_heredocs(command, (*my_env));
    int res = execute_builtin();
    if (res >= 0)
    {
        //free
        return (res);
    }
    execute_external();
    return (0);
}