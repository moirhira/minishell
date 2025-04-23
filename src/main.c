/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moirhira <moirhira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 21:08:03 by moirhira          #+#    #+#             */
/*   Updated: 2025/04/23 14:18:53 by moirhira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../include/minishell.h"
#include "../libraries/libft/libft.h"

void sigint_handler(int signum)
{
    (void)signum;
    rl_replace_line("", 0);
    printf("\n");
    rl_on_new_line();
    rl_redisplay();  
    
}
char *read_input()
{
    char *input;
    input = readline("minishell$ ");
    if (!input)
        exit(EXIT_FAILURE);
    else
        add_history(input);
    return (input);
}

t_token *create_token(char *str, int type)
{
    t_token *new;
    new = (t_token *)malloc(sizeof(t_token));
    if (!new)
        return (NULL);
    new->value = ft_strdup(str);
    new->type = type;
    new->next = NULL;
    return (new);
}
void add_token(t_token **token_lst, t_token *new_token)
{
    t_token *ptr;
    if (!*token_lst)
    {
        *token_lst = new_token;
        return;
    }
    ptr = *token_lst;
    while (ptr->next)
        ptr = ptr->next;
    ptr->next = new_token;
}
void tokinisition(t_token **token, char *command)
{
    int i;
    int j;
    char **arr_commands;
    i = 0;
    arr_commands = split_token(command);
    while(arr_commands[i] != NULL)
    {
        j = 0;
        if(ft_strcmp(arr_commands[i], "|") == 0)
        {
            add_token(token, create_token(arr_commands[i], 1));
        }
        else if(ft_strcmp(arr_commands[i], "<") == 0)
        {
            add_token(token, create_token(arr_commands[i], 2));
        }
        else if(ft_strcmp(arr_commands[i], ">") == 0)
        {
            add_token(token, create_token(arr_commands[i], 3));
        }
        else if(ft_strcmp(arr_commands[i], ">>") == 0)
        {
            add_token(token, create_token(arr_commands[i], 4));
        }
        else if(ft_strcmp(arr_commands[i], "<<") == 0)
        {
            add_token(token, create_token(arr_commands[i], 5));
        }
        else
            add_token(token, create_token(arr_commands[i], 0)); 
        i++;
    }
    
}
void parse_command(t_token **token_list, char *cmd_line)
{
    tokinisition(token_list, cmd_line);
    t_token *ptr;
    ptr = *token_list;
    while (ptr)
    {
        printf("value : %s\n", ptr->value);
        printf("type : %d\n", ptr->type);
        ptr = ptr->next;
    }
    // parsing();
}
void	free_token(t_token **stacka)
{
	t_token *current;
	t_token	*next;

	if (!stacka || !*stacka)
		return ;
	current = *stacka;
	while (current)
	{
		next = current->next;
		free(current);
		current = next;
	}
	*stacka = NULL;
}
int main()
{
    char *cmd_line;
    t_token *token_list;
    // t_command *list_cmd;
    
    token_list = (t_token *)malloc(sizeof(t_token));
    if (!token_list)
        write(2, "Malloc faild!\n", 13);
    token_list = NULL;
    // list_cmd = NULL;
    signal(SIGQUIT, SIG_IGN);
    signal(SIGINT, sigint_handler);
    while (1)
    {
        cmd_line = read_input();
        parse_command(&token_list, cmd_line);
        free_token(&token_list);
    }
    return(0);
}