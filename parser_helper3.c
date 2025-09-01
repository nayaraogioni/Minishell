/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_helper3.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 20:38:07 by dopereir          #+#    #+#             */
/*   Updated: 2025/08/25 22:18:31 by dopereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"

t_command	*finalize_and_return(t_command *cmd, t_pbuilder *pb)
{
	if (finalize_command(cmd, pb) == -1)
		return (NULL);
	return (cmd);
}

/// @brief Prepares environment to parse a pipe.
/// @param pd Pipe data to substitute local variables through functions
/// @param pipeline_cmd Variable we intend to fill through the process
/// @return Returns -1 on failure, 0 on success (ready to parse).
int	pre_pipe_parse(t_pipe_data *pd, t_command **pipeline_cmd)
{
	*pipeline_cmd = init_command();
	if (!*pipeline_cmd)
		return (-1);
	(*pipeline_cmd)->type = T_PIPE;
	(*pipeline_cmd)->commands = malloc(sizeof(t_command *) * MAX_ARGS);
	if (!(*pipeline_cmd)->commands)
	{
		free_command(*pipeline_cmd);
		return (-1);
	}
	pd->i = 0;
	while (pd->i < MAX_ARGS)
	{
		(*pipeline_cmd)->commands[pd->i] = NULL;
		pd->i++;
	}
	pd->start = 0;
	return (0);
}

/// @brief Find and create a sublexer
/// @param pd The pipe data
/// @param lexer The lexer input
/// @param pipeline_cmd Our processing variable
/// @return Return -1 on failure and free's the pipeline_cmd, 0 on success
int	process_sublexer(t_pipe_data *pd, t_lexer *lexer, t_command *pipeline_cmd)
{
	pd->pipe_pos = find_next_pipe(lexer, pd->start);
	if (pd->pipe_pos == -1)
		pd->pipe_pos = lexer->token_count;
	pd->sublexer = create_sublexer(lexer, pd->start, pd->pipe_pos);
	if (!pd->sublexer)
	{
		printf("minishell: syntax error near unexpected token `|'\n");
		free_command(pipeline_cmd);
		return (-1);
	}
	return (0);
}

int	process_leaf(t_pipe_data *pp, t_command *pipeline_cmd, t_env *env,
	t_lexer *lexer)
{
	pp->leaf = parse_simple_command(pp->sublexer, env);
	if (!pp->leaf)
	{
		free_sublexer(pp->sublexer);
		free_command(pipeline_cmd);
		return (-1);
	}
	if (pp->pipe_pos < lexer->token_count)
		pp->leaf->next_is_pipe = 1;
	pipeline_cmd->commands[pipeline_cmd->command_count++] = pp->leaf;
	free_sublexer(pp->sublexer);
	return (0);
}

//Return -1 on failure
//Return 0 on sucess
int	pre_seq_parse(t_seq_data *sd, t_command **sequence_cmd)
{
	*sequence_cmd = init_command();
	if (!*sequence_cmd)
		return (-1);
	(*sequence_cmd)->type = T_AND;
	(*sequence_cmd)->commands = malloc(sizeof(t_command *) * MAX_ARGS);
	if (!(*sequence_cmd)->commands)
	{
		free_command(*sequence_cmd);
		return (-1);
	}
	sd->i = 0;
	while (sd->i < MAX_ARGS)
	{
		(*sequence_cmd)->argv[sd->i] = NULL;
		sd->i++;
	}
	sd->start = 0;
	return (0);
}
