/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 17:22:24 by dopereir          #+#    #+#             */
/*   Updated: 2025/09/09 15:32:10 by dopereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "libft/libft.h"
#include "minishell.h"

int	has_pipes(t_lexer *lexer)
{
	int	i;

	i = 0;
	while (i < lexer->token_count)
	{
		if (lexer->tokens[i].type == T_PIPE)
			return (1);
		i++;
	}
	return (0);
}

//check if has T_VAR type, 1 if has, 0 if don't
int	has_variables(t_lexer *lexer)
{
	int	i;

	i = 0;
	while (i < lexer->token_count)
	{
		if (lexer->tokens[i].type == T_VAR)
			return (1);
		i++;
	}
	return (0);
}

int	has_logical_operators(t_lexer *lexer)
{
	int	i;

	i = 0;
	while (i < lexer->token_count)
	{
		if (lexer->tokens[i].type == T_AND)
			return (1);
		i++;
	}
	return (0);
}

// conta quantos argumentos sao palavras (T_WORD) e nao redirecionamentos
int	count_args(t_lexer *lexer)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (i < lexer->token_count)
	{
		if (lexer->tokens[i].type == T_WORD)
		{
			count++;
			i++;
		}
		else if (lexer->tokens[i].type == T_REDIR_IN
			|| lexer->tokens[i].type == T_REDIR_OUT
			|| lexer->tokens[i].type == T_REDIR_APPEND
			|| lexer->tokens[i].type == T_REDIR_HEREDOC)
		{
			i++;
			if (i < lexer->token_count && lexer->tokens[i].type == T_WORD)
				i++;
		}
		else
			i++;
	}
	return (count);
}

int	find_next_pipe(t_lexer *lexer, int start)
{
	int	i;

	i = start;
	while (i < lexer->token_count)
	{
		if (lexer->tokens[i].type == T_PIPE)
			return (i);
		i++;
	}
	return (-1);
}
