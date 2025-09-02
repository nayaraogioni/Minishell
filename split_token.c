/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_token.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 16:31:44 by nayara            #+#    #+#             */
/*   Updated: 2025/09/02 17:57:22 by dopereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	init_split_tokens(char *str, t_lexer *lexer, char **s, t_token **tokens)
{
	*s = str;
	*tokens = malloc(lexer->token_count * sizeof(t_token));
	if (!*tokens)
		return (0);
	return (1);
}

char	*handle_variable_token(char *s, int *len)
{
	char	*start;

	start = s;
	s++;
	while (*s && (ft_isalnum(*s) || *s == '_' || *s == '?'
			|| *s == '!' || *s == '@' || *s == '#' || *s == '$'))
		s++;
	*len = s - start;
	return (s);
}

char	*handle_double_quotes(char *s, int *len)
{
	char	*start;
	char	*p;

	p = s + 1;
	start = p;
	while (*p)
	{
		if (*p == '\\' && is_escaped_special_char(*(p + 1)))
		{
			p += 2;
			continue ;
		}
		if (*p == '"')
			break ;
		p++;
	}
	*len = p - start;
	if (*p == '"')
		p++;
	return (p);
}

char	*handle_single_quotes(char *s, char quote_char, int *len)
{
	char	*start;

	s++;
	start = s;
	while (*s && *s != quote_char)
		s++;
	*len = s - start;
	if (*s == quote_char)
		s++;
	return (s);
}

char	*handle_quoted_token(char *s, int *len, int *qt_flag)
{
	char	quote_char;

	quote_char = *s;
	if (quote_char == '\'')
		*qt_flag = 1;
	else
		*qt_flag = 2;
	if (quote_char == '"')
		return (handle_double_quotes(s, len));
	else
		return (handle_single_quotes(s, quote_char, len));
}
