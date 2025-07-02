/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_helpers.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 09:32:18 by dopereir          #+#    #+#             */
/*   Updated: 2025/06/25 23:51:25 by dopereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/libft.h"
#include "minishell.h"

char	*ft_strjoin_three(const char *a, const char *b, const char *c)
{
	char	*tmp;
	char	*res;

	tmp = ft_strjoin(a, b);
	res = ft_strjoin(tmp, c);
	free(tmp);
	return (res);
}

//MAY USE THIS FUNCTION TO GENERATE A PATH
char	*cmd_path_generator(char *cmd_name)
{
	char	*path_copy;
	char	*dir;
	char	*command_path;
	char	*result;

	if (!cmd_name || !*cmd_name)
		return (NULL);
	result = NULL;
	path_copy = ft_strdup(getenv("PATH"));
	if (!path_copy)
		return (NULL);
	dir = ft_strtok(path_copy, ":");
	while (dir)
	{
		command_path = ft_strjoin_three(dir, "/", cmd_name);
		if (!command_path)
		{
			dir = ft_strtok(NULL, ":");
			continue ;
		}
		if (access(command_path, X_OK) == 0)
		{
			result = ft_strdup(command_path);
			free (command_path);
			break ;
		}
		free (command_path);
		dir = ft_strtok(NULL, ":");
	}
	free (path_copy);
	return (result);
}

/*int	main(void)
{
	char	*cmd;

	while(1)
	{
		cmd = readline("> ");
		if (ft_strcmp(cmd, "exit") == 0)
			break ;
		if (cmd_path_generator(cmd))
			printf("COMMAND EXISTS!\n");
		else
			printf("command do not exist!\n");
	}
	}*/
