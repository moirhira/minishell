/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_redirections.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekhallaf <ekhallaf@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 06:48:59 by ekhallaf          #+#    #+#             */
/*   Updated: 2025/07/05 06:49:07 by ekhallaf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

ssize_t     ft_getline(char **lineptr, size_t *n, FILE *stream)
{
    size_t pos = 0;
    int c;
    
    if (*lineptr == NULL || *n == 0) {
        *n = 128;  // Initial buffer size
        *lineptr = malloc(*n);
        if (*lineptr == NULL)
            return -1;
    }
    
    while (1) 
    {
        c = fgetc(stream);
        
        /// EOF
        if (c == EOF) {
            if (pos == 0)
                return -1;
            break;
        }
        
        // Resize buffer if needed
        if (pos + 1 >= *n) 
        {
            size_t new_size = *n * 2;
            char *new_ptr = realloc(*lineptr, new_size);
            if (new_ptr == NULL)
                return -1;
            *lineptr = new_ptr;
            *n = new_size;
        }
        (*lineptr)[pos++] = c;
        if (c == '\n')
            break;
    }
    (*lineptr)[pos] = '\0';
    return pos;
}
///////////////////////////////////

void    handle_heredoc(const char *delimiter)
{
    int fd[2];
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    
    if (pipe(fd) == -1)
    {
        perror("pipe");
        exit(EXIT_FAILURE);
    }
    
    while (1)
    {
        printf("heredoc> ");
        read = ft_getline(&line, &len, stdin);
        if (read == -1 || strncmp(line, delimiter, strlen(delimiter)) == 0)
            break;
        write(fd[1], line, read);
    }
    free(line);
    close(fd[1]);
    dup2(fd[0], STDIN_FILENO);
    close(fd[0]);
}

///////////////////////////////////////////////

void setup_redirections(t_command *cmd)
{
    t_redirect *redir = cmd->redirects;
    while (redir)
    {
        if (redir->type == 2) // <
        {
            int fd = open(redir->filename, O_RDONLY);
            dup2(fd, STDIN_FILENO);
            close(fd);
        }
        else if (redir->type == 3) // >
        {
            int fd = open(redir->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            dup2(fd, STDOUT_FILENO);
            close(fd);
        }
        else if (redir->type == 4) // >>
        {
            int fd = open(redir->filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
            dup2(fd, STDOUT_FILENO);
            close(fd);
        }
        else if (redir->type == 5) // << (heredoc)
            handle_heredoc(redir->filename);
        redir = redir->next;
    }
}

