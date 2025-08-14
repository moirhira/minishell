/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moirhira <moirhira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 21:07:57 by moirhira          #+#    #+#             */
/*   Updated: 2025/08/13 23:11:40 by moirhira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

int	handel_pipe(t_token **token, t_command **head, t_command **cmd_lst)
{
	add_command(cmd_lst, creat_command());
	(*head)->pipe = 1;
	*head = (*head)->next;
	*token = (*token)->next;
	return (1);
}

int	parsing_helper(t_token **token, t_command **head, t_command **cmd_lst)
{
	if ((*token)->type == 1)
		return (handel_pipe(token, head, cmd_lst));
	else if ((*token)->type == 2)
		return (handel_input_redirection(token, *head));
	else if ((*token)->type == 3)
		return (handel_output_redirection(token, *head));
	else if ((*token)->type == 4)
		return (handel_append_redirection(token, *head));
	else if ((*token)->type == 5)
		return (handel_heredoc(token, *head));
	else if ((*token)->ignored == 1)
	{
		exit_status(0);
		*token = (*token)->next;
	}
	else
	{
		return (handel_argument(token, *head));
	}
	return (1);
}

t_command	*creat_command(void)
{
	t_command	*new_cmd;

	new_cmd = (t_command *)ft_malloc(sizeof(t_command));
	new_cmd->args = NULL;
	new_cmd->heredoc_count = 0;
	new_cmd->pipe = 0;
	new_cmd->redirects = NULL;
	new_cmd->next = NULL;
	return (new_cmd);
}

t_command	*parsing(t_token **token_lst, t_command **cmd_lst)
{
	t_command	*head;
	t_token		*token;

	head = NULL;
	token = *token_lst;
	while (token)
	{
		if (!head)
		{
			add_command(cmd_lst, creat_command());
			head = *cmd_lst;
		}
		if (!parsing_helper(&token, &head, cmd_lst))
			return (NULL);
	}
	return (head);
}

int	parse_command(t_token **token_lst, t_command **command_lst, char *cmd_line,
		t_envp **my_env)
{
	*token_lst = NULL;
	*command_lst = NULL;
	if (!split_token(cmd_line, my_env, token_lst))
		return (exit_status(2), 2);
	if (!validate_syntaxe(token_lst))
	{
		exit_status(2);
		return (2);
	}
	if (!parsing(token_lst, command_lst))
	{
		return (exit_status(2), 2);
	}
	return (exit_status(-1), 0);
}
