/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_helper_2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nayara <nayara@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/23 12:57:01 by nayara            #+#    #+#             */
/*   Updated: 2025/08/23 13:48:14 by nayara           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_command	*init_pipeline_cmd(void)
{
	t_command	*pipeline_cmd;

	pipeline_cmd = init_command();
	if (!pipeline_cmd)
		return (NULL);
	pipeline_cmd->type = T_PIPE;
	pipeline_cmd->commands = malloc(sizeof(t_command *) * MAX_ARGS);
	if (!pipeline_cmd->commands)
	{
		free_command(pipeline_cmd);
		return (NULL);
	}
	init_commands_array(pipeline_cmd->commands);
	return (pipeline_cmd);
}

int	process_pipeline_loop(t_command *pipeline_cmd, t_lexer *lexer)
{
	t_command	*leaf;
	int			start;
	int			pipe_pos;

	start = 0;
	while (start < lexer->token_count)
	{
		pipe_pos = find_next_pipe(lexer, start);
		if (pipe_pos == -1)
			pipe_pos = lexer->token_count;
		leaf = process_pipe_command(lexer, start, pipe_pos);
		if (!leaf)
			return (0);
		pipeline_cmd->commands[pipeline_cmd->command_count] = leaf;
		pipeline_cmd->command_count++;
		if (pipe_pos == lexer->token_count)
			break ;
		start = pipe_pos + 1;
	}
	return (1);
}

t_command	*parse_pipeline(t_lexer *lexer, t_env *my_env)
{
	t_command	*pipeline_cmd;

	(void)my_env;
	pipeline_cmd = init_pipeline_cmd();
	if (!pipeline_cmd)
		return (NULL);
	if (!process_pipeline_loop(pipeline_cmd, lexer))
	{
		free_command(pipeline_cmd);
		return (NULL);
	}
	return (pipeline_cmd);
}

void	init_argv_array(char **argv)
{
	int	i;

	i = 0;
	while (i < MAX_ARGS)
	{
		argv[i] = NULL;
		i++;
	}
}

t_command	*process_sequence_command(t_lexer *lexer, int start, int and_pos)
{
	t_lexer		*sublexer;
	t_command	*cmd;

	sublexer = create_sublexer(lexer, start, and_pos);
	if (!sublexer)
		return (NULL);
	cmd = parse_function(sublexer, NULL);
	free_sublexer(sublexer);
	return (cmd);
}
