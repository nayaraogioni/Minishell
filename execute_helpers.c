/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_helpers.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 09:32:18 by dopereir          #+#    #+#             */
/*   Updated: 2025/08/21 21:29:12 by dopereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/libft.h"
#include "minishell.h"
#include <linux/limits.h>
#include <readline/chardefs.h>

char	*ft_strjoin_three(const char *a, const char *b, const char *c)
{
	char	*tmp;
	char	*res;

	tmp = ft_strjoin(a, b);
	res = ft_strjoin(tmp, c);
	free(tmp);
	return (res);
}

static void	free_splits(char **arr)
{
	int	i;

	i = 0;
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}

char	*cmd_path_helper(char **paths, char *cmd_name)
{
	char	*tmp;
	int		i;

	i = 0;
	while (paths[i])
	{
		tmp = ft_strjoin_three(paths[i], "/", cmd_name);
		if (tmp && access(tmp, X_OK) == 0)
			return (tmp);
		free(tmp);
		i++;
	}
	return (NULL);
}

//Give a command name and use this function to return a path
char	*cmd_path_generator(char *cmd_name, t_env *env)
{
	char	**paths;
	char	*result;
	char	*custom_path;

	if (!cmd_name || !*cmd_name)
		return (NULL);
	custom_path = ft_getenv(env, "PATH");
	if (!custom_path)
		return (NULL);
	paths = ft_split(custom_path, ':');
	if (!paths)
		return (NULL);
	result = cmd_path_helper(paths, cmd_name);
	free_splits(paths);
	if (result)
		return (result);
	return (cmd_name);
}

//On sucess return 0;
//On failure return -1;
int	replace_env_value(t_env **env, char *key, char *value)
{
	char	*new_key;
	char	*new_value;

	while (*env)
	{
		if (ft_strcmp((*env)->key, key) == 0)
		{
			new_value = ft_strdup(value);
			if (!new_value)
				return (-1);
			free ((*env)->value);
			(*env)->value = new_value;
			return (0);
		}
		env = &(*env)->next;
	}
	new_key = ft_strdup(key);
	if (!new_key)
		return (-1);
	new_value = ft_strdup(value);
	if (!new_value)
		return (free (new_key), -1);
	if (env_add(env, new_key, new_value) != 0)
		return (-1);
	return (0);
}
