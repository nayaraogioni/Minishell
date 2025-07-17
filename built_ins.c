/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_ins.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 17:17:58 by dopereir          #+#    #+#             */
/*   Updated: 2025/07/06 19:16:45 by dopereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <linux/limits.h>

int	ft_cd(char **argv, t_env **env_list)
{
	char	oldpwd[PATH_MAX];
	char	newpwd[PATH_MAX];
	char	*target;

	if (!argv[1] || ft_strcmp(argv[1], "--") == 0)
		target = ft_getenv(*env_list, "HOME");
	else if (ft_strcmp(argv[1], "-") == 0)
	{
		target = ft_getenv(*env_list, "OLDPWD");
		if (target)
			printf("%s\n", target);
	}
	else
		target = argv[1];
	if (!target)
	{
		fprintf(stderr, "minishell: cd: HOME or OLDPWD not set\n");
		return (1);
	}
	if (!getcwd(oldpwd, sizeof(oldpwd)))
	{
		perror("minishell: cd");
		return (1);
	}
	if (chdir(target) != 0)
	{
		perror("minishell: cd");
		return (1);
	}
	if (!getcwd(newpwd, sizeof(newpwd)))
	{
		perror("minishell: cd");
		return (1);
	}
	ft_setenv(env_list, "OLDPWD", oldpwd);
	ft_setenv(env_list, "PWD", newpwd);
	return (0);
}

int	run_parent_built(t_command *cmd, t_env **env_list)
{
	if (!ft_strcmp(cmd->name, "cd"))
		return (ft_cd(cmd->argv, env_list));
	else if (!ft_strcmp(cmd->name, "export"))
		return (ft_export(cmd->argv, env_list));
	else if (!ft_strcmp(cmd->name, "unset"))
		return (ft_unset(cmd->argv, env_list));
	else if (!ft_strcmp(cmd->name, "exit")) /* HERE */
		printf("implement ft_exit\n");
	return (0);
}

bool	is_parent_builtin(char *name)
{
	return (!ft_strcmp(name, "cd") || !ft_strcmp(name, "export")
		|| !ft_strcmp(name, "unset") || !ft_strcmp(name, "exit"));
}

bool	is_any_builtin(char *name)
{
	return (!ft_strcmp(name, "env") || !ft_strcmp(name, "echo")
		|| !ft_strcmp(name, "pwd"));
}
