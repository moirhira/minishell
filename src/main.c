/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moirhira <moirhira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 21:08:03 by moirhira          #+#    #+#             */
/*   Updated: 2025/08/04 21:48:49 by moirhira         ###   ########.fr       */
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
    char *equal_sign;

    head = NULL;
    last = NULL;
    while (env[i] != NULL)
    {
        equal_sign = ft_strchr(env[i], '=');
        if (!equal_sign)
        {
            i++;
            continue;
        }
        new_node = (t_envp *)ft_malloc(sizeof(t_envp));
        if (!new_node)
            return (printf("Error from malloc\n"), NULL);
        size_t key_len = equal_sign - env[i];
        
        new_node->key = (char *)ft_malloc(key_len + 1);
        if (!new_node->key)
            return (printf("Error from malloc\n"), NULL);
        ft_strlcpy(new_node->key, env[i], key_len + 1);
        
        new_node->value = ft_strdup(equal_sign + 1);
        new_node->next = NULL;
        if (!head)
            head = new_node;
        else
            last->next = new_node;
        last = new_node;
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
        // free_all_momory();
        // free_token(&token_list);
        // free_command(&list_cmd);
        // free(cmd_line);
        token_list = NULL;
        list_cmd = NULL;
    }
    free_all_momory();
    // free_command(&list_cmd);
    // free_token(&token_list);
    // free_env(&my_env);
    return(exit_status (-1));
}

