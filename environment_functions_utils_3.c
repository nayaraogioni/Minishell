/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment_functions_utils_3.c                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nayara <nayara@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 01:26:13 by nayara            #+#    #+#             */
/*   Updated: 2025/09/09 03:15:47 by nayara           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	process_export_arg(char **argv, int *i, t_env **env,
						t_parse_data *pd)
{
	char	*eq;
	char	*trimmed;
	char	*current_arg;

	current_arg = argv[*i];
	eq = ft_strchr(current_arg, '=');
	if (identifier_check(eq, current_arg, pd) == 1)
	{
		(*i)++;
		return ;
	}
	if (eq && *(eq + 1) != '\0' && (*(eq + 1) == '"' || *(eq + 1) == '\''))
	{
		trimmed = literal_argv_expander(eq, argv, i);
		if (!trimmed)
		{
			pd->pd_exit_status = 1;
			(*i)++;
			return ;
		}
		add_export(current_arg, env, trimmed);
	}
	else
		export_helper(eq, current_arg, env, pd);
	(*i)++;
}

void	ft_export_loop(char **argv, t_env **env, t_parse_data *pd)
{
	int	i;

	i = 1;
	while (argv[i])
	{
		process_export_arg(argv, &i, env, pd);
	}
}

int	ft_export(char **argv, t_env **env, t_parse_data *pd)
{
	if (!argv || !argv[1])
		return (export_no_args(env, pd));
	ft_export_loop(argv, env, pd);
	return (0);
}
