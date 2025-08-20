/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_token.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nayara <nayara@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 16:31:44 by nayara            #+#    #+#             */
/*   Updated: 2025/08/20 16:36:06 by nayara           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int init_split_tokens(char *str, t_lexer *lexer, char **s, t_token **tokens)
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
	while (*s && (ft_isalnum(*s) || *s == '_' || *s == '?' || 
		*s == '!' || *s == '@' || *s == '#' || *s == '$'))
		s++;
	*len = s - start;
	return (s);
}

char	*handle_double_quotes(char *s, int *len)
{
	char	*start;

	s++;
	start = s;
	while (*s)
	{
		if (*s == '\\' && s[1] != '\0')
		{
			s += 2;
			continue;
		}
		if (*s == '"')
			break;
		s++;
	}
	*len = s - start;
	if (*s == '"')
		s++;
	return (s);
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
	*qt_flag = (quote_char == '\'') ? 1 : 2;
	if (quote_char == '"')
		return (handle_double_quotes(s, len));
	else
		return (handle_single_quotes(s, quote_char, len));
}