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

void	init_cmd_argv(t_command *cmd)
{
	int	i;

	i = 0;
	while (i < MAX_ARGS)
	{
		cmd->argv[i] = NULL;
		i++;
	}
}

t_command	*init_command(void)
{
	t_command	*cmd;

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
	init_cmd_argv(cmd);
	return (cmd);
}

t_command	*parse_simple_command(t_lexer *lexer, t_env *env_list)
{
	t_parse_state	st;

	(void)env_list;
	st.lexer = lexer;
	st.cmd = init_command();
	st.acc = NULL;
	st.i = 0;
	st.arg_index = 0;
	if (!st.cmd || count_args(lexer) == 0)
		return (free_command(st.cmd), NULL);
	while (st.i < lexer->token_count)
	{
		if (is_wordish(&lexer->tokens[st.i]))
		{
			if (!handle_wordish(&st))
				return (free(st.acc), free_command(st.cmd), NULL);
			continue ;
		}
		if (! handle_redirs(&st))
			return (free(st.acc), free_command(st.cmd), NULL);
		st.i++;
	}
	return (finalize_command(&st));
}

void	init_commands_array(t_command **commands)
{
	int	i;

	i = 0;
	while (i < MAX_ARGS)
	{
		commands[i] = NULL;
		i++;
	}
}

t_command	*process_pipe_command(t_lexer *lexer, int start, int pipe_pos)
{
	t_lexer		*sublexer;
	t_command	*leaf;

	sublexer = create_sublexer(lexer, start, pipe_pos);
	if (!sublexer)
	{
		printf("minishell: syntax error near unexpected token `|'\n");
		return (NULL);
	}
	leaf = parse_simple_command(sublexer, NULL);
	if (!leaf)
	{
		free_sublexer(sublexer);
		return (NULL);
	}
	if (pipe_pos < lexer->token_count)
		leaf->next_is_pipe = 1;
	free_sublexer(sublexer);
	return (leaf);
}
