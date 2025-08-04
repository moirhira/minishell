/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sorted_env.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moirhira <moirhira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 22:12:09 by ekhallaf          #+#    #+#             */
/*   Updated: 2025/08/04 19:26:12 by moirhira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

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
		{
			// free_array(arr, i);
			return (1);
		}
		env = env->next;
		i++;
	}
	arr[i] = NULL;
	sort_in_tab(arr, count);
	print_and_free_array(arr);
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

void	print_and_free_array(char **arr)
{
	int	i;

	i = 0;
	while (arr[i])
	{
		write(1, "declare -x ", 11);
		write(1, arr[i], ft_strlen(arr[i]));
		write(1, "\n", 1);
		// free(arr[i]);
		i++;
	}
	// free(arr);
}

void	free_array(char **arr, int size)
{
	int	i;

	i = 0;
	while (i < size)
	{
		// free(arr[i]);
		i++;
	}
	// free(arr);
}
