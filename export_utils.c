/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 20:09:10 by dopereir          #+#    #+#             */
/*   Updated: 2025/09/08 19:48:01 by dopereir         ###   ########.fr       */
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
//TO WORK DIRECTLY WITH t_env *env
void	print_sorted_env(t_env *env)
{
	char	**env_array;
	int		i;

	env_array = env_to_array(env);
	sort_env_array(env_array);
	i = 0;
	while (env_array[i])
	{
		print_export_line(env_array[i]);
		i++;
	}
	free_export_array(env_array);
}

//works with pd->export_env
void	print_export_array(char **arr)
{
	int	i;

	if (!arr)
		return;
	i = 0;
	while (arr[i])
	{
		print_export_line(arr[i]);
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























int	add_to_export_env(char *key, t_parse_data *pd)
{
	int		count;
	char	*new_entry;
	char	**new_array;
	size_t	oldsize;
	size_t	newsize;

	if (!key || !pd)
		return (-1);
	count = 0;
	while (pd->export_env && pd->export_env[count])
	{
		if (ft_strncmp(pd->export_env[count], key, ft_strlen(key)) == 0
			&& pd->export_env[count][ft_strlen(key)] == '=')
			return (0);
		count++;
	}
	new_entry = ft_strdup(key);
	if (!new_entry)
		return (-1);
	oldsize = sizeof(char *) * (count + 1);
	newsize = sizeof(char *) * (count + 2);
	new_array = ft_realloc(pd->export_env, oldsize, newsize);
	if (!new_array)
	{
		free(new_entry);
		return (-1);
	}
	new_array[count] = new_entry;
	new_array[count + 1] = NULL;
	pd->export_env = new_array;
	return (0);
}

int	is_valid_identifier(const char *key)
{
	int	i;

	if (!key || !key[0])
		return (0);
	if (!ft_isalpha(key[0]) && key[0] != '_')
		return (0);
	i = 1;
	while (key[i])
	{
		if (!ft_isalnum(key[i]) && key[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

void	print_identifier_error(const char *key)
{
	write(STDERR_FILENO, "minishell: export: `", 21);
	write(STDERR_FILENO, key, ft_strlen(key));
	write(STDERR_FILENO, "': not a valid identifier\n", 27);
}
