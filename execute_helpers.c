/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_helpers.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 09:32:18 by dopereir          #+#    #+#             */
/*   Updated: 2025/07/06 17:18:43 by dopereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/libft.h"
#include "minishell.h"
#include "parser.h"
#include <linux/limits.h>
#include <time.h>
#include <unistd.h>

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

//Give a command name and use this function to return a path
char	*cmd_path_generator(char *cmd_name)
{
	char	**paths;
	char	*result;
	char	*tmp;
	int		i;

	if (!cmd_name || !*cmd_name)
		return (NULL);
	result = NULL;
	paths = ft_split(getenv("PATH"), ':');
	if (!paths)
		return (NULL);
	i = 0;
	while (paths[i])
	{
		tmp = ft_strjoin_three(paths[i], "/", cmd_name);
		if (tmp && access(tmp, X_OK) == 0)
		{
			result = tmp;
			break ;
		}
		free (tmp);
		i++;
	}
	free_splits(paths);
	return (result);
}
