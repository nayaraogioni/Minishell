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

//MAY USE THIS FUNCTION TO GENERATE A PATH
int	cmd_path_generator(t_lexer *lexer, char *full_path)
{
	int		found_path_flag;
	char	*dir;
	char	*command;
	char	*path_copy;

	path_copy = ft_strdup(getenv("PATH"));
	command = ft_strdup(lexer->tokens[0].text);
	found_path_flag = 0;
	dir = ft_strtok(path_copy, ":");
	while (dir)
	{
		ft_strcpy(full_path, dir);
		ft_strcat(full_path, "/");
		ft_strcat(full_path, command);
		if (access(full_path, X_OK) == 0)
		{
			found_path_flag = 1;
			break ;
		}
		dir = ft_strtok(NULL, ":");
	}
	free (command);
	free (path_copy);
	return (found_path_flag);
}

int	check_command_exists(char *command, t_token_type *type, char *full_path)
{
	char	*path_copy;
	char	*dir;
	int		command_exists;

	command_exists = 0;
	path_copy = ft_strdup(getenv("PATH"));
	dir = ft_strtok(path_copy, ":");
	while (dir)
	{
		ft_strcpy(full_path, dir);
		ft_strcat(full_path, "/");
		ft_strcat(full_path, command);
		if (access(full_path, X_OK) == 0)
		{
			command_exists = 1;
			break ;
		}
		dir = ft_strtok(NULL, ":");
	}
	free (path_copy);
	if (command_exists && type == T_WORD)
	{
		return (command_exists);
	}
	return (0);
}
