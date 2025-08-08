/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export_sorted_env.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moirhira <moirhira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 22:12:09 by ekhallaf          #+#    #+#             */
/*   Updated: 2025/08/07 20:30:54 by moirhira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"


void sort_in_tab(char **array, int size)
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

int count_env(t_envp *env)
{
    int count = 0;
    while (env)
    {
        count++;
        env = env->next;
    }
    return count;
}

int	print_sorted_export(t_envp *env)
{
	int		i;
	int		count;
	char	**arr;

	count = count_env(env);
	arr = ft_malloc(sizeof(char *) * (count + 1));
	if (!arr)
		return (1);
	i = 0;
	while (env && i < count)
	{
		arr[i] = create_export_entry(env->key, env->value);
		if (!arr[i])
			return (1);
		env = env->next;
		i++;
	}
	arr[i] = NULL;
	sort_in_tab(arr, count);
	print_array(arr);
	return (0);
}

char	*create_export_entry(char *key, char *value)
{
	char	*entry;
	int		len;

	len = ft_strlen(key) + 1;
	if (value)
		len += ft_strlen(value) + 3;
	entry = ft_malloc(len);
	if (!entry)
		return (NULL);
	ft_strlcpy(entry, key, len);
	if (value)
	{
		ft_strlcat(entry, "=\"", len);
		ft_strlcat(entry, value, len);
		ft_strlcat(entry, "\"", len);
	}
	return (entry);
}

void	print_array(char **arr)
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
