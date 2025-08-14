/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment_functions_utils.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 17:58:10 by dopereir          #+#    #+#             */
/*   Updated: 2025/07/07 17:58:27 by dopereir         ###   ########.fr       */
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

<<<<<<< HEAD
=======
//prints our whole enviroment list USE AS ENV COMMAND
void	ft_env(t_env *env)
{
	printf("GOT FT_ENV DIRECTLY\n");
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

>>>>>>> 0384cda6b8b7f354a1d6b8ffa81772123f696e80
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
void	ft_env(t_env *env)
{
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

static void	add_export(char *arg, t_env **env)
{
	char	*eq;
	size_t	klen;
	char	*key;
	char	*value;

	eq = ft_strchr(arg, '=');
	klen = eq - arg;
	key = ft_strndup(arg, klen);
	value = ft_strdup(eq + 1);
	if (key && value)
	{
		ft_setenv(env, key, value);
		free(key);
		free(value);
	}
	else
	{
		free(key);
		free(value);
	}
}

int	ft_export(char **argv, t_env **env)
{
	int		exit_code;
	int		i;
	char	*eq;

	i = 1;
	exit_code = 0;
	while (argv[i])
	{
		eq = ft_strchr(argv[i], '=');
		if (argv[i][0] == '\0' || eq == argv[i])
		{
			printf("minishell: export: '%s': not a valid identifier\n",
				argv[i]);
			exit_code = 1;
		}
		else if (eq)
			add_export(argv[i], env);
		else if (!ft_getenv(*env, argv[i]))
			ft_setenv(env, argv[i], "");
		i++;
	}
	return (exit_code);
}
