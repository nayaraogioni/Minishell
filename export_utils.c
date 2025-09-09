/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 20:09:10 by dopereir          #+#    #+#             */
/*   Updated: 2025/09/08 21:35:25 by dopereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//uses bubble sort
void	sort_env_array(char **arr)
{
	int		i;
	int		j;
	char	*tmp;

	i = 0;
	while (arr[i] && arr[i])
	{
		j = i + 1;
		while (arr[j])
		{
			if (ft_strcmp(arr[i], arr[j]) > 0)
			{
				tmp = arr[i];
				arr[i] = arr[j];
				arr[j] = tmp;
			}
			j++;
		}
		i++;
	}
}

char	**generate_export_array(t_env *env)
{
	char	**arr;

	arr = env_to_array(env);
	if (!arr)
		return (NULL);
	sort_env_array(arr);
	return (arr);
}

void	free_export_array(char **arr)
{
	int	i;

	if (!arr)
		return ;
	i = 0;
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}

void	print_export_line(char *env_var)
{
	char	*eq;

	eq = ft_strchr(env_var, '=');
	if (eq)
	{
		*eq = '\0';
		write(STDOUT_FILENO, "declare -x ", 11);
		write(STDOUT_FILENO, env_var, ft_strlen(env_var));
		write(STDOUT_FILENO, "=\"", 2);
		write(STDOUT_FILENO, eq + 1, ft_strlen(eq + 1));
		write(STDOUT_FILENO, "\"\n", 2);
		*eq = '=';
	}
	else
	{
		write(STDOUT_FILENO, "declare -x ", 11);
		write(STDOUT_FILENO, env_var, ft_strlen(env_var));
		write(STDOUT_FILENO, "\n", 1);
	}
}

//works with pd->export_env
void	print_export_array(char **arr)
{
	int	i;

	if (!arr)
		return ;
	i = 0;
	while (arr[i])
	{
		print_export_line(arr[i]);
		i++;
	}
}
