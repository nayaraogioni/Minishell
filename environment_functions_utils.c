/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment_functions_utils.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nayara <nayara@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 17:58:10 by dopereir          #+#    #+#             */
/*   Updated: 2025/09/09 02:18:26 by nayara           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/libft.h"
#include "minishell.h"
#include <stddef.h>
#include <stdio.h>
#include <unistd.h>

//unset a single env
void	ft_unsetenv(t_env **env, char *key)
{
	t_env	*cur;
	t_env	*prev;

	cur = *env;
	prev = NULL;
	while (cur)
	{
		if (ft_strcmp(cur->key, key) == 0)
		{
			if (prev)
				prev->next = cur->next;
			else
				*env = cur->next;
			free(cur->key);
			free(cur->value);
			free(cur);
			return ;
		}
		prev = cur;
		cur = cur->next;
	}
}

//remove one of more vars, uses ft_unsetenv as helper
int	ft_unset(char **argv, t_env **env)
{
	int	exit_code;
	int	i;

	i = 1;
	exit_code = 0;
	while (argv[i])
	{
		if (argv[i][0] == '\0' || ft_strchr(argv[i], '='))
		{
			printf("minishell: unset: '%s': not a valid identifier\n", argv[i]);
			exit_code = 1;
		}
		else
			ft_unsetenv(env, argv[i]);
		i++;
	}
	return (exit_code);
}

//prints our whole enviroment list USE AS ENV COMMAND
void	ft_env(t_env *env, t_command *cmd)
{
	if (cmd->argv[1])
	{
		print_no_file_dir_error(cmd->argv[1]);
		return ;
	}
	while (env)
	{
		if (env->value)
		{
			write(STDOUT_FILENO, env->key, ft_strlen(env->key));
			write(STDOUT_FILENO, "=", 1);
			write(STDOUT_FILENO, env->value, ft_strlen(env->value));
			write(STDOUT_FILENO, "\n", 1);
		}
		env = env->next;
	}
}

int	add_export(char *arg, t_env **env, char *value)
{
	char	*eq;
	size_t	klen;
	char	*key;

	eq = ft_strchr(arg, '=');
	if (!eq)
		return (-1);
	klen = eq - arg;
	key = ft_strndup(arg, klen);
	if (!key)
		return (-1);
	if (replace_env_value(env, key, value) != 0)
	{
		free(key);
		free(value);
		return (-1);
	}
	free(key);
	free(value);
	return (0);
}
