/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_token_helper.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nayara <nayara@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 16:33:44 by nayara            #+#    #+#             */
/*   Updated: 2025/08/20 16:37:59 by nayara           ###   ########.fr       */
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

char	*handle_regular_token(char *s, char delim, int *len)
{
	char	*start;

	start = s;
	while (*s && *s != delim && *s != '|' && *s != '>' && 
		*s != '<' && *s != '&')
		s++;
	*len = s - start;
	return (s);
}

int	is_double_operator(char *s)
{
	return ((*s && *(s + 1)) && ((*s == '>' && *(s + 1) == '>') ||
		(*s == '<' && *(s + 1) == '<') || (*s == '&' && *(s + 1) == '&')));
}

int	is_single_operator(char *s)
{
	return (*s == '|' || *s == '>' || *s == '<' || *s == '$');
}
