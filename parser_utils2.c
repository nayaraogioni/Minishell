/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 22:06:18 by dopereir          #+#    #+#             */
/*   Updated: 2025/08/26 22:55:18 by dopereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_and_null(char **ptr)
{
	if (*ptr)
	{
		free(*ptr);
		*ptr = NULL;
	}
}

void	free_string_array(char **arr, int max)
{
	int	i;

	i = 0;
	while (i < max && arr[i])
	{
		free(arr[i]);
		arr[i] = NULL;
		i++;
	}
}

void	free_command_array(t_command **arr, int count, int max)
{
	int	i;

	i = 0;
	while (i < count && i < max)
	{
		if (arr[i])
		{
			free_command(arr[i]);
			arr[i] = NULL;
		}
		i++;
	}
}

void	free_command_ptr(t_command **ptr)
{
	if (*ptr)
	{
		free_command(*ptr);
		*ptr = NULL;
	}
}

void	free_command(t_command *cmd)
{
	if (!cmd)
		return ;
	free_and_null(&cmd->name);
	free_and_null(&cmd->path);
	free_and_null(&cmd->input_file);
	free_and_null(&cmd->output_file);
	free_and_null(&cmd->hd_delim);
	free_string_array(cmd->argv, MAX_ARGS);
	if (cmd->commands)
	{
		free_command_array(cmd->commands, cmd->command_count, MAX_ARGS);
		free(cmd->commands);
		cmd->commands = NULL;
	}
	free_command_ptr(&cmd->left);
	free_command_ptr(&cmd->right);
	free(cmd);
}
