/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moirhira <moirhira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 21:08:03 by moirhira          #+#    #+#             */
/*   Updated: 2025/07/18 21:59:43 by moirhira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../include/minishell.h"
#include "../libraries/libft/libft.h"

char	*read_input(void)
{
    char *line;
    line = readline("minishell$ ");
    if (!line)
        return (NULL);
    if (*line)
        add_history(line);
    return (line);
}
void	free_split(char **split)
{
	int i = 0;

	if (!split)
		return;
	while (split[i])
		free(split[i++]);
	free(split);
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
    while (env[i] != NULL)
    {
        split = ft_split(env[i], '=');
        if (!split || !split[1])
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
        free_split(split);
        i++;
    }
    return (head);
}


int main(int ac, char **av, char **env)
{
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
            break;
        if (*cmd_line == '\0' || only_whitespace(cmd_line))
        {
            free(cmd_line);
            continue;
        }
        if (parse_command(&token_list, &list_cmd, cmd_line, &my_env) == 0)
            execute_commands(list_cmd,&my_env);
        free_token(&token_list);
        free_command(&list_cmd);
        free(cmd_line);
    }
    free_command(&list_cmd);
    free_token(&token_list);
    free_env(&my_env);
    return(exit_status (-1));
}

