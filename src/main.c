/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moirhira <moirhira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 21:08:03 by moirhira          #+#    #+#             */
/*   Updated: 2025/05/08 22:49:37 by moirhira         ###   ########.fr       */
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
    int len = 0;
    t_envp *envp_struct;
    
    while (env[len])
        len++;
        
    envp_struct = (t_envp *)malloc(sizeof(t_envp));
    if (!envp_struct)
        return(NULL);
    envp_struct->vars = (char **)malloc(sizeof(char *) * len);
    if (!envp_struct->vars )
    {
        free(envp_struct);
        return (NULL);
    }
    while (i < len)
    {
        envp_struct->vars[i] = ft_strdup(env[i]);
        if (!envp_struct->vars[i])
        {
            while (i--)
                free(envp_struct->vars[i]);
            free(envp_struct->vars);
            free(envp_struct);
            return (NULL);
        }
        i++;
    }
    envp_struct->vars[i] = NULL;
    return (envp_struct);
}


int main(int ac, char **av, char **env)
{
    char *cmd_line;
    t_token *token_list;
    t_command *list_cmd;    
    t_envp *my_env ;
    
    my_env = retrieve_envp(env);
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
    // free_arr(my_env);
    return(0);
}


// VAR=VALUE handel this 