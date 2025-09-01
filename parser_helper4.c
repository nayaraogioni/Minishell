/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_helper4.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nayara <nayara@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 20:35:03 by dopereir          #+#    #+#             */
/*   Updated: 2025/09/01 11:32:48 by nayara           ###   ########.fr       */
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

// Returns NULL on error or if no argument found
char	*parse_next_argument(t_lexer *lexer, int *i)
{
	char	*acc;
	char	*tmp;
	t_token	*t;

	acc = NULL;
	while (*i < lexer->token_count && is_wordish(&lexer->tokens[*i]))
	{
		t = &lexer->tokens[*i];
		if (acc && t->join_prev == 0)
			break ;
		tmp = join_words(acc, t->text);
		if (!tmp)
		{
			free (acc);
			return (NULL);
		}
		acc = tmp;
		(*i)++;
		if (*i >= lexer->token_count
			|| !is_wordish(&lexer->tokens[*i])
			|| lexer->tokens[*i].join_prev == 0)
			break ;
	}
	return (acc);
}
