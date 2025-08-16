/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moirhira <moirhira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 08:44:26 by ekhallaf          #+#    #+#             */
/*   Updated: 2025/08/16 09:18:53 by moirhira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

int	builtin_env(t_envp *envp)
{
	t_envp	*env;

	if (!envp)
		return (1);
	env = envp;
	while (env)
	{
		if (env->value)
		{
			ft_putstr_fd(env->key, STDOUT_FILENO);
			ft_putchar_fd('=', STDOUT_FILENO);
			ft_putstr_fd(env->value, STDOUT_FILENO);
			ft_putchar_fd('\n', STDOUT_FILENO);
		}
		env = env->next;
	}
	return (0);
}
