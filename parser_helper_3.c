/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_helper_3.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nayara <nayara@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/23 13:00:59 by nayara            #+#    #+#             */
/*   Updated: 2025/08/23 13:49:35 by nayara           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_command	*init_sequence_cmd(void)
{
	t_command	*sequence_cmd;

	sequence_cmd = init_command();
	if (!sequence_cmd)
		return (NULL);
	sequence_cmd->type = T_AND;
	sequence_cmd->commands = malloc(sizeof(t_command *) * MAX_ARGS);
	if (!sequence_cmd->commands)
	{
		free_command(sequence_cmd);
		return (NULL);
	}
	init_argv_array(sequence_cmd->argv);
	return (sequence_cmd);
}

int	process_sequence_loop(t_command *sequence_cmd, t_lexer *lexer)
{
	t_command	*cmd;
	int			start;
	int			and_pos;

	start = 0;
	while (start < lexer->token_count)
	{
		and_pos = find_next_logical_operator(lexer, start);
		if (and_pos == -1)
			and_pos = lexer->token_count;
		cmd = process_sequence_command(lexer, start, and_pos);
		if (!cmd)
			return (0);
		sequence_cmd->commands[sequence_cmd->command_count] = cmd;
		sequence_cmd->command_count++;
		if (and_pos == lexer->token_count)
			break ;
		start = and_pos + 1;
	}
	return (1);
}

t_command	*parse_sequence(t_lexer *lexer, t_env *my_env)
{
	t_command	*sequence_cmd;

	(void)my_env;
	sequence_cmd = init_sequence_cmd();
	if (!sequence_cmd)
		return (NULL);
	if (!process_sequence_loop(sequence_cmd, lexer))
	{
		free_command(sequence_cmd);
		return (NULL);
	}
	return (sequence_cmd);
}

t_command	*parse_function(t_lexer *lexer, t_env *my_env)
{
	int		flag;

	flag = export_exception_flag(lexer);
	if (has_variables(lexer) && flag != 1)
	{
		if (expand_variables(lexer, my_env) == -1)
		{
			printf("minishell: error expanding variables\n");
			return (NULL);
		}
	}
	if (has_logical_operators(lexer))
		return (parse_sequence(lexer, my_env));
	if (has_pipes(lexer))
		return (parse_pipeline(lexer, my_env));
	else
		return (parse_simple_command(lexer, my_env));
}
