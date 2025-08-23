/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_token_helper.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 16:33:44 by nayara            #+#    #+#             */
/*   Updated: 2025/08/22 14:35:33 by dopereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*handle_double_operators(char *s, int *len)
{
	*len = 2;
	return (s + 2);
}

char	*handle_single_operators(char *s, int *len)
{
	*len = 1;
	return (s + 1);
}

char	*handle_regular_token(char *s, char delim, int *len)//CHANGES HERE
{
	char	*start;
	int		dummy_len;

	start = s;
	while (*s && *s != delim && *s != '|' && *s != '>' \
		&& *s != '<' && *s != '&')
	{
		if (*s == '"' || *s == '\'')
		{
			if (*s == '"')
				s = handle_double_quotes(s, &dummy_len);
			else
				s = handle_single_quotes(s, *s, &dummy_len);
			continue ;
		}
		s++;
	}
	*len = s - start;
	return (s);
}

int	is_double_operator(char *s)
{
	return ((*s && *(s + 1)) && ((*s == '>' && *(s + 1) == '>') \
		|| (*s == '<' && *(s + 1) == '<') || (*s == '&' && *(s + 1) == '&')));
}

int	is_single_operator(char *s)
{
	return (*s == '|' || *s == '>' || *s == '<');
}
