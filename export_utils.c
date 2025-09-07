/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 20:09:10 by dopereir          #+#    #+#             */
/*   Updated: 2025/09/07 20:16:17 by dopereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_sorted_env(t_env *env)
{
	int		len;
	char	**env_array;
	t_env	*curr;
	int		i;

	len = list_lenght(env);
	env_array = malloc(sizeof(char *) * (len + 1));
	curr = env;
	i = 0;
	while (curr)
	{
		env_array[i] = ft_strjoin(curr->key, "=");
		env_array[i] = ft_strjoin_free(env_array[i], curr->value);
		i++;
		curr = curr->next;
	}
	env_array[i] = NULL;

	sort_string_array(env_array);

	for (i = 0; env_array[i]; i++)
	{
		write(STDOUT_FILENO, "declare -x ", 11);
		write(STDOUT_FILENO, env_array[i], ft_strlen(env_array[i]));
		write(STDOUT_FILENO, "\n", 1);
	}

	free_env_array(env_array, len);
}

/*

*/