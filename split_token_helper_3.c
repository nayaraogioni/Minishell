/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_token_helper_3.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 16:39:30 by nayara            #+#    #+#             */
/*   Updated: 2025/09/02 16:52:24 by dopereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*handle_add_token_error(t_token *tokens, int i, t_lexer *lexer)
{
	free_tokens_partial(tokens, i);
	lexer->tokens = NULL;
	lexer->token_count = 0;
	return (NULL);
}

int	process_single_token(t_token_loop *loop, char *str, char delim,
	t_token **tokens)
{
	t_proc_token	pt;

	ft_memset(&pt, 0, sizeof(t_proc_token));
	loop->s = skip_delimiters(loop->s, delim);
	if (*(loop->s) == '\0')
		return (-1);
	pt.tok_begin = loop->s;
	pt.join_prev = should_join_prev(pt.tok_begin, str, delim);
	loop->s = parse_token(loop->s, delim, &pt);
	return (add_token(tokens, loop->i, &pt));
}

t_token	*tokenize_loop(char *str, char delim, t_lexer *lexer,
	t_token *tokens)
{
	t_token_loop	loop;

	loop.s = str;
	loop.i = 0;
	while (*(loop.s) && loop.i < lexer->token_count)
	{
		loop.rc = process_single_token(&loop, str, delim, &tokens);
		if (loop.rc == -1)
			break ;
		if (loop.rc < 0)
			return (handle_add_token_error(tokens, loop.i, lexer));
		loop.i++;
	}
	lexer->token_count = loop.i;
	lexer->tokens = tokens;
	return (tokens);
}

t_token	*split_tokens(char *str, char delim, t_lexer *lexer)
{
	char	*s;
	t_token	*tokens;

	if (!init_split_tokens(str, lexer, &s, &tokens))
		return (NULL);
	return (tokenize_loop(str, delim, lexer, tokens));
}

int	is_escaped_special_char(char next)
{
	return (next == '"' || next == '\\'
		|| next == '$' || next == '`' || next == '\n');
}
