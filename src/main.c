/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moirhira <moirhira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 21:08:03 by moirhira          #+#    #+#             */
/*   Updated: 2025/08/12 09:45:16 by moirhira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../include/minishell.h"
#include "../libraries/libft/libft.h"


int main(int ac, char **av, char **env)
{
    (void)ac;
    (void)av;
    g_signal_received = 0;
    char *cmd_line;
    t_token *token_list;
    t_command *list_cmd;    
    t_envp *my_env ;
    
    token_list = NULL;
    list_cmd = NULL;
    my_env = retrieve_envp(env);
    if (!my_env)
    {
        printf("Error at retrieving envs\n");
        return (exit_status(1));
    }
    setup_signals(SHELL_INTERACTIVE);
    while (1)
    {
        cmd_line = read_input();
        if (!cmd_line)
        {
            free_all_memory();
            fd_collector(-1, 1);
            free(cmd_line);
            exit(exit_status(-1));
        }
        if (*cmd_line == '\0' || only_whitespace(cmd_line))
        {
            free(cmd_line);
            continue;
        }
        
        if (parse_command(&token_list, &list_cmd, cmd_line, &my_env) != 0)
        {
            token_list = NULL;
            list_cmd = NULL;
            continue;
        }
        free(cmd_line);
        if (parse_heredocs(list_cmd, my_env) != 0)
        {
            if (g_signal_received)
            {
                exit_status(130);
                g_signal_received = 0;
            }
            token_list = NULL;
            list_cmd = NULL;
            continue;
        }
        execute_commands(list_cmd,&my_env);
        token_list = NULL;
        list_cmd = NULL;
    }
    fd_collector(-1, 1);
    free_all_memory();
    return(exit_status (-1));
}

