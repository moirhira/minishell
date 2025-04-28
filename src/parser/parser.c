/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moirhira <moirhira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 21:07:57 by moirhira          #+#    #+#             */
/*   Updated: 2025/04/28 16:05:54 by moirhira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_command *creat_command(void)
{
    t_command *new_cmd;
    new_cmd = (t_command *)malloc(sizeof(t_command));
    if (!new_cmd)
        return (NULL);
    new_cmd->append = 0;
    new_cmd->args = NULL;
    new_cmd->herdoc = NULL;
    new_cmd->infile = NULL;
    new_cmd->pipe = 0;
    new_cmd->outfile = NULL;
    return (new_cmd);
}

void add_command(t_command **command_lst, t_command *new_command)
{
    t_command *ptr;
    if (!*command_lst)
    {
        *command_lst = new_command;
        return;
    }
    ptr = *command_lst;
    while (ptr->next)
        ptr = ptr->next;
    ptr->next = new_command;
}
void tokinisition(t_token **token, char *command, char **my_env)
{
    if (!split_token(command, my_env, token))
    {
        free_arr(my_env);
        free_token(token);
        exit(EXIT_FAILURE);
    }
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
                printf("argument num[%d]: %s\n", i, ptr->args[i]);
                i++;
            }
        }
        printf("appending : %d\n", ptr->append);
        printf("hedrdoc : %s\n", ptr->herdoc);
        printf("redirect to file : %s\n", ptr->infile);
        printf("is a pipe : %d\n", ptr->pipe);
        printf("output file : %s\n", ptr->outfile);
        printf("....................................\n");
        ptr = ptr->next;
    }
    printf("here im \n");
}

void add_argument(t_command *cmd, char *arg)
{
    int len;
    char **new_args;
    len = 0;
    if (!cmd->args)
    {
        cmd->args = (char **)malloc(sizeof(char *) * 2);
        if (!cmd->args)
            return;
        cmd->args[0] = ft_strdup(arg);
        cmd->args[1] = NULL ;
        return;
    }
    while (cmd->args[len])
        len++;
    new_args = (char **)malloc(sizeof(char *) * (len + 2));
    if (!new_args)
        return;
    int i = 0;
    while (i < len)
    {
        new_args[i] = cmd->args[i];
        i++;
    }
    new_args[i++] = ft_strdup(arg);
    new_args[i] = NULL;
    free(cmd->args);
    cmd->args = new_args;
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
        if (token->type == 1) // | 
        {
            if (!token->next)
                return (NULL);
            add_command(cmd_lst, creat_command());
            head->pipe = 1;
            head = head->next;
            token = token->next;
        }
        else if (token->type == 2) // < 
        {
            if (!token->next)
                return (NULL);
            token = token->next;
            head->infile = ft_strdup(token->value);
            token = token->next;
        }
        else if (token->type == 4) // > and  >>
        {
            if (!token->next)
                return (NULL);
            token = token->next;
            head->outfile = ft_strdup(token->value);
            token = token->next;
        }
        else if (token->type == 5) // << herdoc
        {
            if (!token->next)
                return (NULL);
            token = token->next;
            head->herdoc = ft_strdup(token->value);
            token = token->next;
        }
        else
        {
            add_argument(head, token->value);
            token = token->next;
        }
    }
    return (*cmd_lst);
}
void parse_command(t_token **token_lst, t_command **command_lst, char *cmd_line, char **my_env)
{
    tokinisition(token_lst, cmd_line, my_env);
    if (!parsing(token_lst, command_lst))
    {
        printf("msh: syntax error near unexpected token `newline'\n");
        free_command(command_lst);
        free_token(token_lst);
        free_arr(my_env);
        exit (EXIT_FAILURE);
    }
    print_commands(command_lst);
}














// t_token *ptr;
    // ptr = *token_lst;
    // while (ptr)
    // {
    //     printf("value : %s\n", ptr->value);
    //     printf("type : %d\n", ptr->type);
    //     printf("attached : %d\n", ptr->attached);
    //     printf("....................................\n");
    //     ptr = ptr->next;
    // }