/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nayara <nayara@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 17:22:24 by dopereir          #+#    #+#             */
/*   Updated: 2025/08/23 13:23:05 by nayara           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

void	free_argv(t_command *cmd)
{
	int	i;

	i = 0;
	while (i < MAX_ARGS && cmd->argv[i])
	{
		free(cmd->argv[i]);
		cmd->argv[i] = NULL;
		i++;
	}
}
