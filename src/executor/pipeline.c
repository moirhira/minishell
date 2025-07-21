/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moirhira <moirhira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 23:01:03 by ekhallaf          #+#    #+#             */
/*   Updated: 2025/07/21 20:22:14 by moirhira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// exec
void exec_command(char **args)
{
    if (!args || !args[0])
    {
        printf("Empty command\n");
        exit(1);
    }
    execvp(args[0], args);
    perror("execvp failed");
    exit(1);
}

// pipeline 
void execute_pipeline(t_command *cmd_list)
{
    int prev_pipe = -1;
    int pipefd[2];
    t_command *cmd = cmd_list;

    while (cmd)
    {
        if (cmd->pipe)
        {
            if (pipe(pipefd) == -1)
            {
                perror("pipe");
                exit(1);
            }
        }

        pid_t pid = fork();
        if (pid == -1)
        {
            perror("fork");
            exit(1);
        }

        if (pid == 0) // Child process
        {
            if (prev_pipe != -1)
            {
                dup2(prev_pipe, STDIN_FILENO); // input from previous pipe
                close(prev_pipe);
            }

            if (cmd->pipe)
            {
                close(pipefd[0]); // Close unused read end
                dup2(pipefd[1], STDOUT_FILENO); // output to pipe
                close(pipefd[1]);
            }

            exec_command(cmd->args); // Replace process image
        }
        // Parent
        if (prev_pipe != -1)
            close(prev_pipe);
            
        if (cmd->pipe)
        {
            close(pipefd[1]);
            prev_pipe = pipefd[0];
        }
        cmd = cmd->next;
    }

    while (wait(NULL) > 0);
}



char **split_args(char *cmd)
{
    char **args = malloc(sizeof(char *) * 10);
    int i = 0;
    char *token = strtok(cmd, " ");
    while (token && i < 9)
    {
        args[i++] = strdup(token);
        token = strtok(NULL, " ");
    }
    args[i] = NULL;
    return args;
}

t_command *parse_pipeline(char *input)
{
    t_command *head = NULL;
    t_command *current = NULL;

    char *copy = strdup(input); // copy input to safely modify it
    char *token = strtok(copy, "|");

    while (token)
    {
        // trim leading/trailing spaces
        while (*token == ' ') token++;
        char *end = token + strlen(token) - 1;
        while (end > token && *end == ' ') *end-- = '\0';

        t_command *cmd = malloc(sizeof(t_command));
        cmd->args = split_args(token);
        cmd->pipe = 1;  // default to pipe = 1
        cmd->next = NULL;

        if (!head)
            head = cmd;
        else
            current->next = cmd;
        current = cmd;

        token = strtok(NULL, "|");
    }

    if (current)
        current->pipe = 0; // last command doesn't pipe

    free(copy);
    return head;
}

// int main(int argc, char **argv)
// {
//     if (argc != 2)
//     {
//         printf("Usage: %s \"command1 | command2 | ...\"\n", argv[0]);
//         return 1;
//     }
//     t_command *cmd_list = parse_pipeline(argv[1]);
//     execute_pipeline(cmd_list);
//     return 0;
// }


// Helper to create a new t_command node

// t_command *new_command(char *cmd_line, int has_pipe)
// {
//     t_command *cmd = malloc(sizeof(t_command));
//     if (!cmd)
//         return NULL;

//     cmd->args = malloc(sizeof(char *) * 10);
//     if (!cmd->args)
//         return NULL;

//     char *copy = strdup(cmd_line); // ✅ copy the string literal
//     if (!copy)
//         return NULL;

//     int i = 0;
//     char *token = strtok(copy, " ");
//     while (token && i < 9)
//     {
//         cmd->args[i++] = strdup(token);
//         token = strtok(NULL, " ");
//     }
//     cmd->args[i] = NULL;

//     free(copy);  // ✅ cleanup

//     cmd->heredoc_count = 0;
//     cmd->infile_count = 0;
//     cmd->outfile_count = 0;
//     cmd->append_count = 0;
//     cmd->pipe = has_pipe;
//     cmd->redirects = NULL;
//     cmd->next = NULL;

//     return cmd;
// }

// int main()

// {
//     t_command *c1 = new_command("ls", 1);
//     t_command *c2 = new_command("grep .c", 1);
//     t_command *c3 = new_command("wc -l", 0);

//     c1->next = c2;
//     c2->next = c3;

//     execute_pipeline(c1);
    
//     return 0;
// }