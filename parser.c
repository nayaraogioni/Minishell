/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nayara <nayara@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/07 14:37:35 by nayara            #+#    #+#             */
/*   Updated: 2025/06/07 14:37:37 by nayara           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "libft/libft.h"
#include "minishell.h"
#include <stdio.h>
#include <time.h>

t_command	*init_command(void)
{
	t_command	*cmd;
	int			i;

	cmd = malloc(sizeof(t_command));
	if (!cmd)
		return (NULL);
	setup_command_defaults(cmd);
	return (cmd);
}

t_command	*parse_simple_command(t_lexer *lexer, t_env *env_list)
{
	t_command	*cmd;
	t_pbuilder	pb;

	(void)env_list;
	cmd = init_command();
	ft_memset(&pb, 0, sizeof(pb));
	if (!cmd)
		return (NULL);
	pb.args_count = count_args(lexer);
	if (pb.args_count == 0)
		return (free_command(cmd), NULL);
	while (pb.i < lexer->token_count)
	{
		pb.res = parse_section1(cmd, lexer, &pb);
		if (pb.res == -1)
			return (NULL);
		if (pb.res == 2)
			return (finalize_and_return(cmd, &pb));
		if (pb.res == 1)
			continue ;
		pb.i++;
	}
	return (finalize_and_return(cmd, &pb));
}

t_command	*parse_pipeline(t_lexer *lexer, t_env *my_env)
{
	t_pipe_data	pipe_data;
	t_command	*pipeline_cmd;

	ft_memset(&pipe_data, 0, sizeof(pipe_data));
	if (pre_pipe_parse(&pipe_data, &pipeline_cmd) == -1)
		return (NULL);
	while (pipe_data.start < lexer->token_count)
	{
		if (process_sublexer(&pipe_data, lexer, pipeline_cmd) == -1)
			return (NULL);
		if (process_leaf(&pipe_data, pipeline_cmd, my_env, lexer) == -1)
			return (NULL);
		if (pipe_data.pipe_pos == lexer->token_count)
			break ;
		pipe_data.start = pipe_data.pipe_pos + 1;
	}
	return (pipeline_cmd);
}

t_command	*parse_sequence(t_lexer *lexer, t_env *my_env)
{
	t_seq_data	sd;
	t_command	*sequence_cmd;

	ft_memset(&sd, 0, sizeof(sd));
	if (pre_seq_parse(&sd, &sequence_cmd) == -1)
		return (NULL);
	while (sd.start < lexer->token_count)
	{
		if (process_sequence_segment(&sd, sequence_cmd, lexer, my_env) == -1)
			return (NULL);
		if (sd.op_pos == lexer->token_count)
			break ;
		sd.start = sd.op_pos + 1;
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
