/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_ins.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 17:17:58 by dopereir          #+#    #+#             */
/*   Updated: 2025/07/19 02:12:13 by dopereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/libft.h"
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
		return (1);
	if (!getcwd(oldpwd, sizeof(oldpwd)))
		return (perror("minishell: cd"), 1);
	if (chdir(target) != 0)
		return (perror("minishell: cd"), 1);
	if (!getcwd(newpwd, sizeof(newpwd)))
		return (perror("minishell: cd"), 1);
	replace_env_value(env_list, "OLDPWD", oldpwd);
	replace_env_value(env_list, "PWD", newpwd);
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
	else if (!ft_strcmp(cmd->name, "exit"))
		printf("implement ft_exit\n");
	else if (!ft_strcmp(cmd->name, "env"))
		return (ft_env(*env_list), 0);
	return (0);
}

bool	is_parent_builtin(char *name)
{
	return (!ft_strcmp(name, "cd") || !ft_strcmp(name, "export")
		|| !ft_strcmp(name, "unset") || !ft_strcmp(name, "exit")
		|| !ft_strcmp(name, "env"));
}

bool	is_any_builtin(char *name)
{
	return (!ft_strcmp(name, "echo") || !ft_strcmp(name, "pwd"));
}
