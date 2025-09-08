/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils_2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 21:32:29 by dopereir          #+#    #+#             */
/*   Updated: 2025/09/08 22:55:20 by dopereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*int	add_to_export_env(char *key, t_parse_data *pd)
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
}*/

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

int	export_no_args(t_env **env, t_parse_data *pd)
{
	pd->export_env = generate_export_array(*env);
	if (!pd->export_env)
	{
		pd->pd_exit_status = 1;
		return (-1);
	}
	print_export_array(pd->export_env);
	free_export_array(pd->export_env);
	pd->export_env = NULL;
	pd->pd_exit_status = 0;
	return (0);
}

//RETURN 1 means continue;
//RETUNR 0 means ignore;
int	identifier_check(char *eq, char *argv, t_parse_data *pd)
{
	char	*key;

	if (!eq)
	{
		if (!is_valid_identifier(argv))
		{
			print_identifier_error(argv);
			pd->pd_exit_status = 1;
			return (1);
		}
		return (1);
	}
	key = ft_substr(argv, 0, eq - argv);
	if (!key || !is_valid_identifier(key))
	{
		print_identifier_error(argv);
		if (key)
			free(key);
		pd->pd_exit_status = 1;
		return (1);
	}
	if (key)
		free(key);
	return (0);
}

int	export_trimmed_value(char *eq, char **argv, int *i, t_env **env)
{
	char	*trimmed;

	trimmed = literal_argv_expander(eq, argv, i);
	if (!trimmed)
	{
		(*i)++;
		return (1);
	}
	add_export(argv[*i], env, trimmed);
	return (0);
}
