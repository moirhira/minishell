/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokinizition.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moirhira <moirhira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 11:14:16 by moirhira          #+#    #+#             */
/*   Updated: 2025/08/17 09:55:42 by moirhira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKINIZITION_H
# define TOKINIZITION_H
# include "../../../include/minishell.h"

typedef struct s_quoted_command
{
	char	quote;
	char	*final_str;
	t_envp	**my_env;
}			t_quoted_command;

typedef struct s_tokinizer
{
	t_envp	**my_env;
	t_token	**token;
	int		state;
}			t_tokinizer;

typedef struct s_simple_command
{
	char	*simple_str;
	int		attached;
	int		dont_trim;
	int		unclose_quote;
	t_envp	**my_env;
	t_token	**token;
}			t_simple_command;

typedef struct s_herdoc_command
{
	char	*delimiter_val;
	int		k;
	int		was_quote;
}			t_herdoc_command;

#endif