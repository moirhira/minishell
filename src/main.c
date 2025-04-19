/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moirhira <moirhira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 21:08:03 by moirhira          #+#    #+#             */
/*   Updated: 2025/04/19 21:51:51 by moirhira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "parser/parser.c"
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

int main()
{
    signal(SIGQUIT, SIG_IGN);
    signal(SIGINT, sigint_handler);
    char *command;
    while (1)
    {
        command = read_input();
        printf("%s", command);
    }
    return(0);
}