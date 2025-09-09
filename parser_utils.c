/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nayara <nayara@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 17:22:24 by dopereir          #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2025/09/09 17:21:10 by nayara           ###   ########.fr       */
=======
/*   Updated: 2025/09/09 15:32:10 by dopereir         ###   ########.fr       */
>>>>>>> 5fe75188209e5c772b3fc472f083b677515b9b15
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

int	has_variables(t_lexer *lexer)
{
	int	i;

	i = 0;
	while (i < lexer->token_count)
	{
		if (i > 0 && lexer->tokens[i -1].type == T_REDIR_HEREDOC)
		{
			i++;
			continue ;
		}
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
