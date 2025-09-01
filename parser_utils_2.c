/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils_2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nayara <nayara@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/23 11:20:49 by nayara            #+#    #+#             */
/*   Updated: 2025/08/23 13:18:38 by nayara           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_arrays(t_command *cmd)
{
	int	i;

	i = 0;
	while (i < cmd->command_count && i < MAX_ARGS)
	{
		if (cmd->commands[i])
		{
			free_command(cmd->commands[i]);
			cmd->commands[i] = NULL;
		}
		i++;
	}
	free(cmd->commands);
	cmd->commands = NULL;
}

void	free_cmds_strings(t_command *cmd)
{
	if (cmd->name)
	{
		free(cmd->name);
		cmd->name = NULL;
	}
	if (cmd->path)
	{
		free(cmd->path);
		cmd->path = NULL;
	}
	if (cmd->input_file)
	{
		free(cmd->input_file);
		cmd->input_file = NULL;
	}
	if (cmd->output_file)
	{
		free(cmd->output_file);
		cmd->output_file = NULL;
	}
	if (cmd->hd_delim)
	{
		free(cmd->hd_delim);
		cmd->hd_delim = NULL;
	}
}

void	free_command(t_command *cmd)
{
	if (!cmd)
		return ;
	free_argv(cmd);
	if (cmd->commands)
		free_arrays(cmd);
	if (cmd->left)
	{
		free_command(cmd->left);
		cmd->left = NULL;
	}
	if (cmd->right)
	{
		free_command(cmd->right);
		cmd->right = NULL;
	}
	free(cmd);
}

int	find_next_pipe(t_lexer *lexer, int start)
{
	int	i;

	i = start;
	while (i < lexer->token_count)
	{
		if (lexer->tokens[i].type == T_PIPE)
			return (i);
		i++;
	}
	return (-1);
}

int	find_next_logical_operator(t_lexer *lexer, int start)
{
	int	i;

	i = start;
	while (i < lexer->token_count)
	{
		if (lexer->tokens[i].type == T_AND)
			return (i);
		i++;
	}
	return (-1);
}
