/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export_sorted_env.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moirhira <moirhira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 22:12:09 by ekhallaf          #+#    #+#             */
/*   Updated: 2025/08/11 21:21:54 by moirhira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"


static void sort_in_tab(char **array, int size)
{
    int i;
	char *temp;
	
	i = 0;
    while (i < size - 1)
    {
        int j = i + 1;
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
	int	i;

	i = 0;
	while (arr[i])
	{
		ft_putstr_fd("declare -x ", STDOUT_FILENO);
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


