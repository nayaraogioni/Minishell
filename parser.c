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
	cmd->type = T_WORD;
	cmd->name = NULL;
	cmd->path = NULL;
	cmd->input_file = NULL;
	cmd->output_file = NULL;
	cmd->pid_filename_output = 0;
	cmd->command_count = 0;
	cmd->commands = NULL;
	cmd->left = NULL;
	cmd->right = NULL;
	cmd->next_is_pipe = 0;
	cmd->next_is_and = 0;
	cmd->hd_delim = NULL;
	cmd->heredoc_fd = -1;
	i = 0;
	while (i < MAX_ARGS)
	{
		cmd->argv[i] = NULL;
		i++;
	}
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
	t_command	*sequence_cmd;
	t_lexer		*sublexer;
	int		start;
	int		and_pos;
	int		i;

	sequence_cmd = init_command();
	if (!sequence_cmd)
		return NULL;
	sequence_cmd->type = T_AND;
	sequence_cmd->commands = malloc(sizeof(t_command *) * MAX_ARGS);
	if (!sequence_cmd->commands)
	{
		free_command(sequence_cmd);
		return NULL;
	}
	i = 0;
	while (i < MAX_ARGS)
	{
		sequence_cmd->argv[i] = NULL;
		i++;
	}
	start = 0;
	while (start < lexer->token_count)
	{
		and_pos = find_next_logical_operator(lexer, start);
		if (and_pos == -1)
			and_pos = lexer->token_count; // se nao encontrar pega o final
		sublexer = create_sublexer(lexer, start, and_pos);
		if (!sublexer)
		{
			free_command(sequence_cmd);
			return NULL;
		}
		sequence_cmd->commands[sequence_cmd->command_count] = parse_function(sublexer, my_env);
		if (!sequence_cmd->commands[sequence_cmd->command_count])
		{
			free_sublexer(sublexer);
			free_command(sequence_cmd);
			return NULL;
		}
		sequence_cmd->command_count++;
		free_sublexer(sublexer);
		if (and_pos == lexer->token_count)
			break; // se for o ultimo comando sai do loop
		start = and_pos + 1; // pula o operador logico para o proximo
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
			return NULL;
		}
	}
	if (has_logical_operators(lexer))
		return parse_sequence(lexer, my_env);
	if (has_pipes(lexer))
		return parse_pipeline(lexer, my_env);
	else
		return parse_simple_command(lexer, my_env);
}
