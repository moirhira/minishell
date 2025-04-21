/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moirhira <moirhira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 21:08:03 by moirhira          #+#    #+#             */
/*   Updated: 2025/04/21 22:35:21 by moirhira         ###   ########.fr       */
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

// t_node	*push(char *str, t_stack *stacka)
// {
// 	t_node	*new_number;
// 	t_node	*ptr;

// 	new_number = (t_node *)malloc(sizeof(t_node));
// 	if (!new_number)
// 		return (NULL);
// 	new_number->value = ft_atoi(str);
// 	new_number->next = NULL;
// 	if (stacka->top == NULL)
// 	{
// 		stacka->top = new_number;
// 		stacka->size++;
// 	}
// 	else
// 	{
// 		ptr = stacka->top;
// 		while (ptr->next)
// 			ptr = ptr->next;
// 		ptr->next = new_number;
// 		stacka->size++;
// 	}
// 	return (stacka->top);
// }
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
    arr_commands = ft_split(command, ' ');
    while(arr_commands[i] != NULL)
    {
        j = 0;
        if(ft_strchr(arr_commands[i], '|')) // pipe
        {
            if (ft_strlen(arr_commands[i]) > 1)
            {
                exit(1);
            }
            add_token(token, create_token("|", 1));
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
int main()
{
    char *cmd_line;
    t_token *token_list;
    // t_command *list_cmd;
    
    token_list = (t_token *)malloc(sizeof(t_token));
    if (!token_list)
        write(2, "Malloc faild!\n", 13);
    token_list->next = NULL;
    // list_cmd = NULL;
    signal(SIGQUIT, SIG_IGN);
    signal(SIGINT, sigint_handler);
    while (1)
    {
        cmd_line = read_input();
        parse_command(&token_list, cmd_line);
        
    }
    return(0);
}