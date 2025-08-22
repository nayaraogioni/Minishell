/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_token_helper_3.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 16:39:30 by nayara            #+#    #+#             */
/*   Updated: 2025/08/22 14:45:39 by dopereir         ###   ########.fr       */
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

int	process_single_token(char **s, char *str, char delim,
	t_token **tokens, int i)
{
	char	*start;
	char	*tok_begin;
	int		len;
	int		qt_flag;
	int		join_prev;

	*s = skip_delimiters(*s, delim);
	if (**s == '\0')
		return (-1);
	tok_begin = *s;
	join_prev = should_join_prev(tok_begin, str, delim);
	*s = parse_token(*s, delim, &start, &len, &qt_flag);
	return (add_token(tokens, i, start, len, qt_flag, join_prev));
}

t_token	*tokenize_loop(char *str, char delim, t_lexer *lexer,
	t_token *tokens)
{
	char	*s;
	int		i;
	int		rc;

	s = str;
	i = 0;
	while (*s && i < lexer->token_count)
	{
		rc = process_single_token(&s, str, delim, &tokens, i);
		if (rc == -1)
			break ;
		if (rc < 0)
			return (handle_add_token_error(tokens, i, lexer));
		i++;
	}
	lexer->token_count = i;
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
