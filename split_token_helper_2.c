/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_token_helper_2.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 16:38:23 by nayara            #+#    #+#             */
/*   Updated: 2025/08/22 13:18:44 by dopereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*parse_variable_or_quote(char *s, char **start, \
	int *len, int *qt_flag)
{
	if (*s == '$' && *(s + 1))
	{
		*start = s;
		return (handle_variable_token(s, len));
	}
	else if (*s == '"' || *s == '\'')
	{
		s++;
		*start = s;
		s--;
		return (handle_quoted_token(s, len, qt_flag));
	}
	return (NULL);
}

char	*parse_operators_or_regular(char *s, char delim, char **start, \
	int *len)
{
	if (is_double_operator(s))
	{
		*start = s;
		return (handle_double_operators(s, len));
	}
	else if (is_single_operator(s))
	{
		*start = s;
		return (handle_single_operators(s, len));
	}
	else
	{
		*start = s;
		return (handle_regular_token(s, delim, len));
	}
}

char	*parse_token(char *s, char delim, char **start, \
	int *len, int *qt_flag)
{
	char	*result;

	*qt_flag = 0;
	result = parse_variable_or_quote(s, start, len, qt_flag);
	if (result)
		return (result);
	return (parse_operators_or_regular(s, delim, start, len));
}

char	*skip_delimiters(char *s, char delim)
{
	while (*s && *s == delim)
		s++;
	return (s);
}

int	should_join_prev(char *tok_begin, char *str, char delim)
{
	if (tok_begin > str && *(tok_begin - 1) != delim)
		return (1);
	return (0);
}
