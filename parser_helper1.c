/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_helper1.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nayara <nayara@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/23 20:14:30 by dopereir          #+#    #+#             */
/*   Updated: 2025/09/01 11:37:45 by nayara           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	redir_fd_helper(t_command *cmd)
{
	int	fd;

	fd = open(cmd->output_file, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd >= 0)
		close(fd);
}

static int	redir_out_helper(t_command *cmd, t_lexer *lexer, t_pbuilder *pb)
{
	if (cmd->output_file)
	{
		redir_fd_helper(cmd);
		free(cmd->output_file);
	}
	if (!lexer->tokens[pb->i].text)
	{
		free(pb->acc);
		free_command(cmd);
		return (-1);
	}
	cmd->output_file = ft_strdup(lexer->tokens[pb->i].text);
	if (!cmd->output_file)
	{
		free(pb->acc);
		free_command(cmd);
		return (-1);
	}
	if (pb->was_append)
		cmd->type = T_REDIR_APPEND;
	else
		cmd->type = T_REDIR_OUT;
	pb->i++;
	return (1);
}

int	redir_out_module(t_command *cmd, t_lexer *lexer, t_pbuilder *pb)
{
	pb->was_append = 0;
	if (lexer->tokens[pb->i].type == T_REDIR_APPEND)
		pb->was_append = 1;
	pb->i++;
	if (pb->i < lexer->token_count && lexer->tokens[pb->i].type == T_WORD)
	{
		return (redir_out_helper(cmd, lexer, pb));
	}
	if (pb->i < lexer->token_count)
		argument_redirs_error(lexer->tokens[pb->i].type);
	else
		printf("minishell: syntax error near unexpected token `newline'\n");
	free(pb->acc);
	free_command(cmd);
	return (-1);
}

///NOTE: return (-1) means continue;
/// return -1 on failure
int	redir_in_module(t_command *cmd, t_lexer *lexer, t_pbuilder *pb)
{
	pb->i++;
	if (pb->i < lexer->token_count && lexer->tokens[pb->i].type == T_WORD)
	{
		if (cmd->input_file)
			free(cmd->input_file);
		cmd->input_file = ft_strdup(lexer->tokens[pb->i].text);
		if (!cmd->input_file)
		{
			free (pb->acc);
			free_command(cmd);
			return (-1);
		}
		pb->i++;
		return (1);
	}
	if (pb->i < lexer->token_count)
		argument_redirs_error(lexer->tokens[pb->i].type);
	else
		printf("minishell: syntax error near unexpected token `newline'\n");
	free(pb->acc);
	free_command(cmd);
	return (-1);
}

///NOTE: This module parses a "wordish" token to the cmd struct
//return 1 on sucess
//return -1 on failure
int	is_wordish_module(t_command *cmd, t_lexer *lexer, t_pbuilder *pb)
{
	char	*arg;

	arg = parse_next_argument(lexer, &pb->i);
	if (!arg)
	{
		free_command(cmd);
		return (-1);
	}
	if (cmd->name == NULL)
	{
		cmd->name = ft_strdup(arg);
		if (!cmd->name)
		{
			free(arg);
			free_command(cmd);
			return (-1);
		}
	}
	if (pb->arg_index < MAX_ARGS - 1)
		cmd->argv[pb->arg_index++] = arg;
	else
		free(arg);
	return (1);
}
