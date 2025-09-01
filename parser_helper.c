/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_helper.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nayara <nayara@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/23 12:54:39 by nayara            #+#    #+#             */
/*   Updated: 2025/08/23 13:55:52 by nayara           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_wordish_helper(t_parse_state *st, t_token *t)
{
	st->cmd->name = ft_strdup(t->text);
	if (!st->cmd->name)
		return (0);
	if (st->arg_index < MAX_ARGS - 1)
	{
		st->cmd->argv[st->arg_index++] = ft_strdup(t->text);
		if (!st->cmd->argv[st->arg_index])
			return (0);
		st->arg_index++;
	}
	st->i++;
	return (1);
}

int	handle_wordish(t_parse_state *st)
{
	t_token	*t;
	char	*tmp;

	t = &st->lexer->tokens[st->i];
	if (st->cmd->name == NULL)
	{
		handle_wordish_helper(st, t);
		return (1);
	}
	tmp = join_words(st->acc, t->text);
	if (!tmp)
		return (0);
	st->acc = tmp;
	if (st->i + 1 >= st->lexer->token_count
		|| !is_wordish(&st->lexer->tokens[st->i + 1])
		|| st->lexer->tokens[st->i + 1].join_prev == 0)
	{
		if (st->arg_index < MAX_ARGS - 1)
			st->cmd->argv[st->arg_index++] = st->acc;
		else
			free(st->acc);
		st->acc = NULL;
	}
	st->i++;
	return (1);
}

int	handle_redirs_helper(t_parse_state *st)
{
	if (st->cmd->name == NULL)
	{
		st->cmd->name = ft_strdup(st->acc);
		if (!st->cmd->name)
			return (0);
	}
	if (st->arg_index < MAX_ARGS - 1)
		st->cmd->argv[st->arg_index++] = st->acc;
	else
		free(st->acc);
	st->acc = NULL;
	return (1);
}

// Substitua a função handle_redirs por esta versão simplificada:

int handle_redirs(t_parse_state *st)
{
	//int was_append;

	if (st->lexer->tokens[st->i].type == T_REDIR_IN
		|| st->lexer->tokens[st->i].type == T_REDIR_OUT
		|| st->lexer->tokens[st->i].type == T_REDIR_APPEND
		|| st->lexer->tokens[st->i].type == T_REDIR_HEREDOC)
	{
		if (st->acc)
		{
			if (!handle_redirs_helper(st))
				return (0);
		}
		// Aqui você pode adicionar a lógica específica de cada tipo de redirecionamento
		// Por agora, apenas pula o token de redirecionamento
		st->i++;
		if (st->i < st->lexer->token_count)
			st->i++; // pula também o arquivo/delimitador
	}
	else if (st->lexer->tokens[st->i].type == T_AND)
	{
		if (st->acc)
		{
			if (!handle_redirs_helper(st))
				return (0);
		}
		st->cmd->argv[st->arg_index] = NULL;
		return (-1);
	}
	return (1);
}

/* int	handle_redirs(t_parse_state *st)
{
	int	was_append;

	if (st->lexer->tokens[st->i].type == T_REDIR_IN)
		return (handle_redir_in(st));
	else if (st->lexer->tokens[st->i].type == T_REDIR_OUT
		|| st->lexer->tokens[st->i].type == T_REDIR_APPEND)
	{
		was_append = (st->lexer->tokens[st->i].type == T_REDIR_APPEND);
		return (handle_redir_out(st, was_append));
	}
	else if (st->lexer->tokens[st->i].type == T_REDIR_HEREDOC)
		return (handle_heredoc(st));
	else if (st->lexer->tokens[st->i].type == T_AND)
	{
		if (st->acc)
		{
			if (!handle_redirs_helper(st))
				return (0);
		}
		st->cmd->argv[st->arg_index] = NULL;
		return (-1);
	}
	return (1);
} */

t_command	*finalize_command(t_parse_state *st)
{
	if (st->acc)
	{
		if (st->cmd->name == NULL)
		{
			st->cmd->name = ft_strdup(st->acc);
			if (!st->cmd->name)
				return (free(st->acc), free_command(st->cmd), NULL);
		}
		if (st->arg_index < MAX_ARGS - 1)
			st->cmd->argv[st->arg_index++] = st->acc;
		else
			free(st->acc);
		st->acc = NULL;
	}
	st->cmd->argv[st->arg_index] = NULL;
	if (st->cmd->name == NULL && st->arg_index == 0
		&& !st->cmd->input_file && !st->cmd->output_file
		&& !st->cmd->hd_delim)
	{
		free_command(st->cmd);
		return (NULL);
	}
	return (st->cmd);
}
