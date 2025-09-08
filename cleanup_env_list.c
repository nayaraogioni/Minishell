/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup_env_list.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 22:35:00 by dopereir          #+#    #+#             */
/*   Updated: 2025/09/07 22:56:30 by dopereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/libft.h"
#include "minishell.h"

void	clean_env_list(t_env **env_list)
{
	t_env	*cur;
	t_env	*next;

	if (!env_list)
		return ;
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

//return 0 on sucess, 1 on failure
int	env_to_array_helper(t_env *tmp, char **arr)
{
	const char	*val;
	size_t		klen;
	char		*s;
	int			i;

	i = 0;
	while (tmp)
	{
		if (tmp->value)
			val = tmp->value;
		else
			val = "";
		klen = ft_strlen(tmp->key);
		s = malloc(klen + 1 + ft_strlen(val) + 1);
		if (!s)
			return (free_env_array(arr, i), 1);
		ft_memcpy(s, tmp->key, klen);
		s[klen] = '=';
		ft_memcpy(s + klen + 1, val, ft_strlen(val));
		s[klen + 1 + ft_strlen(val)] = '\0';
		arr[i++] = s;
		tmp = tmp->next;
	}
	return (0);
}

char	**env_to_array(t_env *env)
{
	int		count;
	t_env	*tmp;
	char	**arr;

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
	tmp = env;
	if (env_to_array_helper(tmp, arr) == 1)
		return (NULL);
	return (arr);
}
