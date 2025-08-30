/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils3.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 22:16:44 by dopereir          #+#    #+#             */
/*   Updated: 2025/08/26 23:01:58 by dopereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

void	free_sublexer(t_lexer *sublexer)
{
	int	i;

	if (!sublexer)
		return ;
	if (sublexer->tokens)
	{
		i = 0;
		while (i < sublexer->token_count)
		{
			if (sublexer->tokens[i].text)
				free(sublexer->tokens[i].text);
			i++;
		}
		free(sublexer->tokens);
	}
	free(sublexer);
}

static t_lexer	*prep_sublexer(t_lexer *lexer, int start, int end)
{
	t_lexer	*sublexer;

	sublexer = malloc(sizeof(t_lexer));
	if (!sublexer)
		return (NULL);
	ft_memset(sublexer, 0, sizeof(t_lexer));
	sublexer->input = lexer->input;
	sublexer->token_count = end - start;
	if (sublexer->token_count <= 0)
	{
		free(sublexer);
		return (NULL);
	}
	sublexer->tokens = malloc(sizeof(t_token) * sublexer->token_count);
	if (!sublexer->tokens)
	{
		free(sublexer);
		return (NULL);
	}
	ft_memset(sublexer->tokens, 0,
		sizeof(t_token) * sublexer->token_count);
	return (sublexer);
}

t_lexer	*create_sublexer(t_lexer *lexer, int start, int end)
{
	t_sub_data	d;

	d.sublexer = prep_sublexer(lexer, start, end);
	if (!d.sublexer)
		return (NULL);
	d.j = 0;
	d.i = start;
	while (d.i < end)
	{
		d.sublexer->tokens[d.j].type = lexer->tokens[d.i].type;
		d.sublexer->tokens[d.j].text = ft_strdup(lexer->tokens[d.i].text);
		if (!d.sublexer->tokens[d.j].text)
		{
			free_tokens_partial(d.sublexer->tokens, d.j);
			free(d.sublexer);
			return (NULL);
		}
		d.j++;
		d.i++;
	}
	return (d.sublexer);
}

void	setup_command_defaults(t_command *cmd)
{
	int	i;

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
}
