/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_helper4.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 20:35:03 by dopereir          #+#    #+#             */
/*   Updated: 2025/08/26 20:46:26 by dopereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	process_sequence_segment(t_seq_data *sd, t_command *sequence_cmd,
									t_lexer *lexer, t_env *env)
{
	sd->op_pos = find_next_logical_operator(lexer, sd->start);
	if (sd->op_pos == -1)
		sd->op_pos = lexer->token_count;
	sd->sublexer = create_sublexer(lexer, sd->start, sd->op_pos);
	if (!sd->sublexer)
	{
		free_command(sequence_cmd);
		return (-1);
	}
	sequence_cmd->commands[sequence_cmd->command_count]
		= parse_function(sd->sublexer, env);
	if (!sequence_cmd->commands[sequence_cmd->command_count])
	{
		free_sublexer(sd->sublexer);
		free_command(sequence_cmd);
		return (-1);
	}
	sequence_cmd->command_count++;
	free_sublexer(sd->sublexer);
	return (0);
}
