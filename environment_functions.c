/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment_functions.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 20:26:58 by dopereir          #+#    #+#             */
/*   Updated: 2025/07/06 23:21:37 by dopereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	env_add(t_env **head, char *key, char *value)
{
	t_env	*node;

	node = malloc(sizeof(t_env));
	if (!node)
	{
		free (key);
		free (value);
		return ;
	}
	node->key = key;
	node->value = value;
	node->next = *head;

	fprintf(stderr, "[env_add] node=%p key=%p '%s' value=%p '%s'\n",
		(void *)node, (void *)key, key ? key : "(null)",
		(void *)value, value ? value : "(null)");

	*head = node;
}

static void	env_init_helper(t_env **env, char *env_str)
{
	char	*eq;
	char	*key;
	char	*value;

	eq = ft_strchr(env_str, '=');
	if (!eq)
		return ;
	key = ft_strndup(env_str, eq - env_str);
	value = ft_strdup(eq + 1);
	if (!key || !value || *key == '\0')
	{
		free(key);
		free(value);
		return ;
	}
	env_add(env, key, value);
}

void	env_init(t_env **my_env, char **envp)
{
	int		i;

	i = 0;
	while (envp[i])
	{
		env_init_helper(my_env, envp[i]);
		i++;
	}
}

//assumes that env is correctly created and populated with var envs
//replicates original getenv behaviour
char	*ft_getenv(t_env *env, char *key)
{
	while (env)
	{
		if (ft_strcmp(env->key, key) == 0)
			return (env->value);
		env = env->next;
	}
	return (NULL);
}

//FUNCTION TO REPLICATED EXPORT command
// invetigate: this can create duplicate nodes?
void	ft_setenv(t_env **env, char *key, char *value)
{
	t_env	*cur;

	cur = *env;
	while (cur)
	{
		if (ft_strcmp(cur->key, key) == 0)
		{
			free(cur->value);
			cur->value = ft_strdup(value);
			return ;
		}
		cur = cur->next;
	}
	env_add(env, ft_strdup(key), ft_strdup(value));
}
