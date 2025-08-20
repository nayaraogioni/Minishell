/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_var_helpers2.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 20:56:50 by dopereir          #+#    #+#             */
/*   Updated: 2025/08/19 20:56:58 by dopereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//-1 on failure, (it frees the char *out), caller must return NULL
// 1 or 2 on sucess, it represent index consumed, the caller must increment
int	backslash_helper(int i, char **out, char *src, int quot)
{
	char	next;

	next = src[i + 1];
	if (quot == 2)
	{
		if (next == '$' || next == '`' || next == '"' || next == '\\')
		{
			if (append_char(out, next) < 0)
				return (-1);
			return (2);
		}
		if (append_char(out, '\\') < 0)
			return (-1);
		return (1);
	}
	if (next != '\0')
	{
		if (append_char(out, next) < 0)
			return (-1);
		return (2);
	}
	return (1);
}

int	process_backslash(int i, char **out, char *src, int quot)
{
	int	consumed;

	if (src[i + 1] == '\0')
	{
		if (append_char(out, '\\') < 0)
			return (-1);
		return (1);
	}
	consumed = backslash_helper(i, out, src, quot);
	return (consumed);
}

//Handles "$?" -> last exit code
char	*last_exit_expander(int last_status, char **out)
{
	char	*num;
	char	*tmp;

	num = ft_itoa(last_status);
	if (num)
		tmp = join_words(*out, num);
	else
		tmp = join_words(*out, "");
	if (num)
		free(num);
	if (!tmp)
	{
		return (NULL);
	}
	*out = tmp;
	return (*out);
}
