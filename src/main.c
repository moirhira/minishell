/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moirhira <moirhira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 21:08:03 by moirhira          #+#    #+#             */
/*   Updated: 2025/04/28 14:43:09 by moirhira         ###   ########.fr       */
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
char **retrieve_envp(char **env)
{
    int i = 0;
    int len = 0;
    while (env[len])
        len++;
    char **my_envp = (char **)malloc(sizeof(char *) * (len + 1));
    if (!my_envp)
        return (NULL);
    while (i < len)
    {
        my_envp[i] = ft_strdup(env[i]);
        if (!my_envp[i])
        {
            while (i--)
                free(my_envp[i]);
            free(my_envp);
            return (NULL);
        }
        i++;
    }
    my_envp[i] = NULL;
    return (my_envp);
}


int main(int ac, char **av, char **env)
{
    char *cmd_line;
    t_token *token_list;
    t_command *list_cmd;
    char **my_env = retrieve_envp(env);
    token_list = (t_token *)malloc(sizeof(t_token));
    if (!token_list)
        write(2, "Malloc faild!\n", 13);
    token_list = NULL;
    list_cmd = NULL;
    signal(SIGQUIT, SIG_IGN);
    signal(SIGINT, sigint_handler);
    while (1)
    {
        cmd_line = read_input();
        parse_command(&token_list, &list_cmd, cmd_line, my_env);
        free(cmd_line);
        free_token(&token_list);
        free_command(&list_cmd);
    }
    free_arr(my_env);
    return(0);
}
