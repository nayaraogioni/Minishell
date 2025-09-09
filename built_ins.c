/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_ins.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 17:17:58 by dopereir          #+#    #+#             */
/*   Updated: 2025/09/09 15:57:55 by dopereir         ###   ########.fr       */
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

///NOTE: if returns 1; the input is freed here
int	ft_exit(char *input)
{
	char	*ptr;
	int		exit_code;

	exit_code = 0;
	ptr = input;
	while (*ptr && ft_isspace((unsigned char)*ptr))
		ptr++;
	if (ft_strncmp(ptr, "exit", 4) == 0 && (ptr[4] == '\0'
			|| ft_isspace((unsigned char)ptr[4])))
	{
		ptr += 4;
		while (*ptr && ft_isspace((unsigned char)*ptr))
			ptr++;
		if (*ptr)
			exit_code = ft_exit_helper(ptr);
		free (input);
		if (exit_code == 0)
			return (EXIT_NO_ARG);
		else
			return (exit_code);
	}
	return (0);
}

int	run_parent_built(t_command *cmd, t_env **env_list, t_parse_data *pd)
{
	if (!ft_strcmp(cmd->name, "cd"))
		return (ft_cd(cmd->argv, env_list));
	else if (!ft_strcmp(cmd->name, "export"))
		return (ft_export(cmd->argv, env_list, pd));
	else if (!ft_strcmp(cmd->name, "unset"))
		return (ft_unset(cmd->argv, env_list));
	else if (!ft_strcmp(cmd->name, "pwd"))
		return (ft_pwd(env_list, pd, cmd));
	else if (!ft_strcmp(cmd->name, "env"))
		return (ft_env(*env_list, cmd), 0);
	else if (!ft_strcmp(cmd->name, "echo"))
		return (ft_echo(pd, cmd));
	return (0);
}

bool	is_parent_builtin(char *name)
{
	return (!ft_strcmp(name, "cd") || !ft_strcmp(name, "export")
		|| !ft_strcmp(name, "unset") || !ft_strcmp(name, "exit")
		|| !ft_strcmp(name, "env") || !ft_strcmp(name, "pwd")
		|| !ft_strcmp(name, "echo"));
}

int	ft_pwd(t_env **env, t_parse_data *pd, t_command *cmd)
{
	char	*pwd;

	(void)cmd;
	pwd = ft_getenv(*env, "PWD");
	if (pwd)
	{
		write(STDOUT_FILENO, pwd, ft_strlen(pwd));
		write(STDOUT_FILENO, "\n", 1);
		pd->pd_exit_status = 0;
	}
	return (1);
}
