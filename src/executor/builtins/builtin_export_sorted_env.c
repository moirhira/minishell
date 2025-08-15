/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export_sorted_env.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moirhira <moirhira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 22:12:09 by ekhallaf          #+#    #+#             */
/*   Updated: 2025/08/15 22:47:21 by moirhira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

static void	sort_in_tab(char **array, int size)
{
	int		i;
	char	*temp;
	int		j;

	i = 0;
	while (i < size - 1)
	{
		j = i + 1;
		while (j < size)
		{
			if (ft_strcmp(array[i], array[j]) > 0)
			{
				temp = array[i];
				array[i] = array[j];
				array[j] = temp;
			}
			j++;
		}
		i++;
	}
}

static void	print_array(char **arr)
{
	int		i;
	char	*equal_sign;

	i = 0;
	while (arr[i])
	{
		ft_putstr_fd("declare -x ", STDOUT_FILENO);
		equal_sign = ft_strchr(arr[i], '=');
		if (equal_sign)
		{
			write(STDOUT_FILENO, arr[i], equal_sign - arr[i] + 1);
			ft_putstr_fd("\"", STDOUT_FILENO);
			ft_putstr_fd(equal_sign + 1, STDOUT_FILENO);
			ft_putstr_fd("\"", STDOUT_FILENO);
		}
		else
			ft_putstr_fd(arr[i], STDOUT_FILENO);
		ft_putstr_fd("\n", STDOUT_FILENO);
		i++;
	}
}

int	print_sorted_export(t_envp *env)
{
	int		count;
	char	**arr;

	count = ft_lstsize(env);
	arr = convert_env_to_array(env);
	sort_in_tab(arr, count);
	print_array(arr);
	return (0);
}
