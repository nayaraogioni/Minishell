/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup_env_list.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 22:35:00 by dopereir          #+#    #+#             */
/*   Updated: 2025/07/07 22:35:01 by dopereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	clean_env_list(t_env **env_list)
{
	t_env	*cur;
	t_env	*next;

	cur = *env_list;
	while (cur)
	{
		next = cur->next;
		free(cur->key);
		free(cur->value);
		free(cur);
		cur = next;
	}
	*env_list = NULL;
}

int	list_lenght(t_env *env_list)
{
	int		count;

	count = 0;
	while (env_list)
	{
		count++;
		env_list = env_list->next;
	}
	return (count);
}

void	free_env_array(char **arr, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}

char	**env_to_array(t_env *env)
{
	int		count;
	t_env	*tmp;
	char	**arr;
	int		i;

	count = 0;
	tmp = env;
	while (tmp)
	{
		count++;
		tmp = tmp->next;
	}
	arr = malloc((count + 1) * sizeof(char *));
	if (!arr)
		return (NULL);
	arr[count] = NULL;

	// 3) Fill the array with "KEY=VALUE" strings
	tmp = env;
	i = 0;
	while (tmp)
	{
		const char	*val = (tmp->value) ? tmp->value : "";
		size_t		klen = ft_strlen(tmp->key);
		size_t		vlen = ft_strlen(val);
		char		*s = malloc(klen + 1 + vlen + 1);

		if (!s)
		{
			free_env_array(arr, i);
			return NULL;
		}

		ft_memcpy(s, tmp->key, klen);
		s[klen] = '=';
		ft_memcpy(s + klen + 1, val, vlen);
		s[klen + 1 + vlen] = '\0';

		arr[i++] = s;
		tmp = tmp->next;
	}

	return (arr);
}
