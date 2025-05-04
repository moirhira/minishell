/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moirhira <moirhira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 21:07:57 by moirhira          #+#    #+#             */
/*   Updated: 2025/05/04 15:13:21 by moirhira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_command *creat_command(void)
{
    t_command *new_cmd;
    new_cmd = (t_command *)malloc(sizeof(t_command));
    if (!new_cmd)
        return (NULL);
    new_cmd->args = NULL;
    new_cmd->heredocs = NULL;
    new_cmd->heredoc_count = 0;
    new_cmd->infiles = NULL;
    new_cmd->infile_count = 0;
    new_cmd->outfiles = NULL;
    new_cmd->outfile_count = 0;
    new_cmd->append = 0;
    new_cmd->pipe = 0;
    new_cmd->next = NULL;
    return (new_cmd);
}

void add_command(t_command **command_lst, t_command *new_command)
{
    t_command *ptr;
    if (!command_lst || !new_command)
        return;
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
    split_token(command, my_env, token);
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
        printf("appending     : %d\n", ptr->append);
        i = 0;
        printf("output files  :\n");
        while (i < ptr->outfile_count)
        {
            printf("     number %d -> : %s\n", i + 1, ptr->outfiles[i]);
            i++;
        }
        i = 0;
        printf("input files   :\n");
        while (i < ptr->infile_count)
        {
            printf("     number %d -> : %s\n", i + 1, ptr->infiles[i]);
            i++;
        }
        i = 0;
        printf("herdocs       :\n");
        while (i < ptr->heredoc_count)
        {
            printf("     number %d -> : %s\n", i + 1 , ptr->heredocs[i]);
            i++;
        }
        
        printf("is a pipe     : %d\n", ptr->pipe);
        printf("....................................\n");
        ptr = ptr->next;
    }
}

void add_argument(t_command *cmd, char *arg)
{
    int len;
    char **new_args;
    len = 0;
    if (!arg)
        return;
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

int check_next_token(t_token *token, t_command *head)
{
    if (!token->next)
    {
        printf("minishell: syntax error near unexpected token `newline'\n");
        free(head);
        return (0);
    }
    if (token->next->type != 0)
    {
        printf("minishell: syntax error near unexpected token `%s'\n", token->next->value); 
        free(head);
        return (0);
    }
    return (1);
}

int handel_pipe(t_token **token, t_command **head, t_command **cmd_lst)
{
    if (!check_next_token(*token, *head))
        return (0);
    add_command(cmd_lst, creat_command());
    (*head)->pipe = 1;
    *head = (*head)->next;
    *token = (*token)->next;
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
        if((*token_lst)->type != 0)
        {
            printf("minishell: syntax error near unexpected token `newline'\n");
            free(head);
            return (NULL);
        }
        if (token->type == 1 && !handel_pipe(&token, &head, cmd_lst)) // | 
            return (NULL);
        else if (token->type == 2) // < 
        {
            if (!check_next_token(token, head))
                return (NULL);
            token = token->next;
            char **new_arr = realloc(head->infiles, sizeof(char *) * (head->infile_count + 2));
            if (!new_arr)
            {
                printf("error from realloc!\n");
                exit(1);
            }
            head->infiles = new_arr;
            head->infiles[head->infile_count] = ft_strdup(token->value);
            head->infile_count++;
            head->infiles[head->infile_count] = NULL; 
            token = token->next;
        }
        else if (token->type == 4 || token->type == 3) // >> and  >
        {
            if (!check_next_token(token, head))
                return (NULL);
            if (token->type == 4)
                head->append = 1;
            if (token->type == 3)
                head->append = 2;
            token = token->next;
            char **new_arr = realloc(head->outfiles, sizeof(char *) * (head->outfile_count + 2));
            if (!new_arr)
            {
                printf("error from realloc!\n");
                exit(1);
            }
            head->outfiles = new_arr;
            head->outfiles[head->outfile_count] = ft_strdup(token->value);
            head->outfile_count++;
            head->outfiles[head->outfile_count] = NULL; 
            token = token->next;
        }
        else if (token->type == 5) // << herdoc
        {
            if (!check_next_token(token, head))
                return (NULL);
            token = token->next;
            char **new_arr = realloc(head->heredocs, sizeof(char *) * (head->heredoc_count + 2));
            if (!new_arr)
            {
                printf("error from realloc!\n");
                exit(1);
            }
            head->heredocs = new_arr;
            head->heredocs[head->heredoc_count] = ft_strdup(token->value);
            head->heredoc_count++;
            head->heredocs[head->heredoc_count] = NULL; 
            token = token->next;
        }
        else
        {
            char *full_str = ft_strdup(token->value);
            while (token->attached)
            {
                token = token->next;
                full_str = ft_strjoin(full_str, token->value);
            }
            add_argument(head, full_str);
            free(full_str);
            token = token->next;
        }
    }
    return (t_command *)(1);
}
void parse_command(t_token **token_lst, t_command **command_lst, char *cmd_line, char **my_env)
{
    tokinisition(token_lst, cmd_line, my_env);
    if (!parsing(token_lst, command_lst))
    {
        free_token(token_lst);
        free_arr(my_env);
        // free_command(command_lst);
        exit (EXIT_FAILURE);
    }
    print_commands(command_lst);
}




// > append overight 1
// >> apend 2 

// ls -l > hello.c >> ty.v 
/*
echo $
# → empty string or warn user (depends on your logic)

echo $$
# → syntax error (unless you implement PID)

echo $!
# → syntax error (unless you implement job control)

echo $?  ✅

*/





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
    

/*
if (!token->next )
            {
                printf("minishell: syntax error near unexpected token `newline'\n");
                free(head);
                return (NULL);
            }
            if (token->next->type != 0)
            {
                printf("minishell: syntax error near unexpected token `%s'\n", token->next->value);
                free(head);
                return (NULL);
            }
*/