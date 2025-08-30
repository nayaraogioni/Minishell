/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 09:28:33 by dopereir          #+#    #+#             */
/*   Updated: 2025/08/27 22:04:48 by dopereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "libft/libft.h"
#include "minishell.h"
#include <stddef.h>
#include <time.h>

// func para validar se todas as aspas estão fechadas
int	validate_quotes(char *str)
{
	bool	in_single_quote;
	bool	in_double_quote;
	char	*s;

	in_single_quote = false;
	in_double_quote = false;
	s = str;
	while (*s)
	{
		if (*s == '\'' && !in_double_quote)
			in_single_quote = bool_changer(in_single_quote);
		else if (*s == '"' && !in_single_quote)
			in_double_quote = bool_changer(in_double_quote);
		s++;
	}
	if (in_single_quote)
		return (printf("minishell: syntax error: unclosed single quote\n"), 0);
	if (in_double_quote)
	{
		printf("minishell: syntax error: unclosed double quote\n");
		return (0);
	}
	return (1);
}

static void	token_counter_quoted(t_token_counter *tc)
{
	tc->quote_char = *(tc->s);
	tc->s++;
	while (*(tc->s) && *(tc->s) != tc->quote_char)
		tc->s++;
	if (*(tc->s) == tc->quote_char)
		tc->s++;
}

static void	token_counter_unquoted(t_token_counter *tc, char delim)
{
	if ((*(tc->s) && *(tc->s + 1))
		&& ((*(tc->s) == '>' && *(tc->s + 1) == '>')
			|| (*(tc->s) == '<' && *(tc->s + 1) == '<')
			|| (*(tc->s) == '&' && *(tc->s + 1) == '&')))
	{
		tc->s += 2;
	}
	else if (*(tc->s) == '|' || *(tc->s) == '>' || *(tc->s) == '<')
		tc->s++;
	else
	{
		while (*(tc->s) && *(tc->s) != delim && *(tc->s) != '|'
			&& *(tc->s) != '>' && *(tc->s) != '<' && *(tc->s) != '&')
			tc->s++;
	}
}

int	token_counter(char *str, char delim)
{
	t_token_counter	tc;

	tc.s = str;
	tc.res = 0;
	while (*(tc.s))
	{
		while (*(tc.s) && *(tc.s) == delim)
			tc.s++;
		if (*(tc.s) == '\0')
			break ;
		tc.res++;
		if (*(tc.s) == '"' || *(tc.s) == '\'')
			token_counter_quoted(&tc);
		else
			token_counter_unquoted(&tc, delim);
	}
	return (tc.res);
}

t_token_type	determine_type(char *token_text, int qt_flag)
{
	t_token_type	type;
	int				i;

	if (!token_text || !*token_text)
		return (T_WORD);
	type = determine_type_helper(token_text);
	if (type != T_WORD)
		return (type);
	if (qt_flag == 1)
		return (T_WORD);
	i = 0;
	while (token_text[i])
	{
		if (token_text[i] == '$')
			return (T_VAR);
		i++;
	}
	return (T_WORD);
}
