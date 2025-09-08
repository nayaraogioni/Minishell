/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment_functions.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 20:26:58 by dopereir          #+#    #+#             */
/*   Updated: 2025/09/08 21:37:22 by dopereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/libft.h"
#include "minishell.h"

//In case of malloc fail, env_add free the vars
// On sucess returns (0); Failure returns (-1);
int	env_add(t_env **head, char *key, char *value)
{
	t_env	*node;

	node = malloc(sizeof(t_env));
	if (!node)
	{
		free (key);
		free (value);
		return (-1);
	}
	node->key = key;
	node->value = value;
	node->next = *head;
	*head = node;
	return (0);
}

//0 on sucess
//-1 on failure
static int	env_init_helper(t_env **env, char *env_str)
{
	char	*eq;
	char	*key;
	char	*value;

	eq = ft_strchr(env_str, '=');
	if (!eq)
		return (-1);
	key = ft_strndup(env_str, eq - env_str);
	value = ft_strdup(eq + 1);
	if (!key || !value || key[0] == '\0')
	{
		free(key);
		free(value);
		return (-1);
	}
	return (env_add(env, key, value));
}

//in case of failure already cleans the env_list
int	env_init(t_env **my_env, char **envp, t_parse_data *pd)
{
	int		i;

	(void) pd;
	if (!my_env)
		return (-1);
	*my_env = NULL;
	i = 0;
	while (envp && envp[i])
	{
		if (env_init_helper(my_env, envp[i]) != 0)
		{
			clean_env_list(my_env);
			return (-1);
		}
		i++;
	}
	return (0);
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
