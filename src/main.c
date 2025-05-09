/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moirhira <moirhira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 21:08:03 by moirhira          #+#    #+#             */
/*   Updated: 2025/05/09 19:16:38 by moirhira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../include/minishell.h"
#include "../libraries/libft/libft.h"

int g_last_exit_status = 0;
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
t_envp  *retrieve_envp(char **env)
{
    int i = 0;
    t_envp *head;
    t_envp *last;
    t_envp *new_node;
    char **split;

    head = NULL;
    last = NULL;
    while (env[i])
    {
        split = ft_split(env[i], '=');
        if (!split || !split[0] || !split[1])
            return (printf ("error at spliting\n"),NULL);
        new_node = (t_envp *)malloc(sizeof(t_envp));
        if (!new_node)
            return (printf("Error from malloc\n"), NULL);
        new_node->key = ft_strdup(split[0]);
        new_node->value = ft_strdup(split[1]);
        new_node->next = NULL;
        if (!head)
            head = new_node;
        else
            last->next = new_node;
        last = new_node;
        i++;
        free(split[0]);
        free(split[1]);
        free(split);
    }
    return (head);
}


int main(int ac, char **av, char **env)
{
    char *cmd_line;
    t_token *token_list;
    t_command *list_cmd;    
    t_envp *my_env ;
    
    my_env = retrieve_envp(env);
    if (!my_env)
        printf("Erorr at revieving envsn\n");
    token_list = (t_token *)malloc(sizeof(t_token));
    if (!token_list)
        write(2, "Malloc faild!\n", 13);
    token_list = NULL;
    
    list_cmd = (t_command *)malloc(sizeof(t_command));
    if (!list_cmd)
        write(2, "Malloc faild!\n", 13);
    list_cmd = NULL;
    signal(SIGQUIT, SIG_IGN); // /
    signal(SIGINT, sigint_handler);
    while (1)
    {
        cmd_line = read_input();
        parse_command(&token_list, &list_cmd, cmd_line, &my_env);
        free(cmd_line);
        free_command(&list_cmd);
        free_token(&token_list);
    }
    free_env(&my_env);
    return(0);
}


// VAR=VALUE handel this 