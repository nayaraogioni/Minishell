/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils_3.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/23 11:22:25 by nayara            #+#    #+#             */
/*   Updated: 2025/09/01 23:24:23 by dopereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	free_sublexer_tokens(t_lexer *sublexer, int count)
{
	while (count > 0)
		free(sublexer->tokens[--count].text);
	free(sublexer);
}

t_lexer	*cloning_lexer(t_lexer *lexer, int start, int end)
{
	int		j;
	t_lexer	*sublexer;

	j = 0;
	sublexer = malloc(sizeof(t_lexer));
	if (!sublexer)
		return (NULL);
	while (start < end)
	{
		sublexer->tokens[j].type = lexer->tokens[start].type;
		sublexer->tokens[j].text = ft_strdup(lexer->tokens[start].text);
		if (!sublexer->tokens[j].text)
		{
			free_sublexer_tokens(sublexer, j);
			return (NULL);
		}
		j++;
		start++;
	}
	return (sublexer);
}

t_lexer	*create_sublexer(t_lexer *lexer, int start, int end)
{
	t_lexer	*sublexer;

	sublexer = malloc(sizeof(t_lexer));
	if (!sublexer)
		return (NULL);
	sublexer->input = lexer->input;
	sublexer->token_count = end - start;
	if (sublexer->token_count <= 0)
	{
		free(sublexer);
		return (NULL);
	}
	sublexer->tokens = malloc(sizeof(t_token) * sublexer->token_count);
	if (!sublexer->tokens)
	{
		free(sublexer);
		return (NULL);
	}
	cloning_lexer(lexer, start, end);
	return (sublexer);
}

void	free_sublexer(t_lexer *sublexer)
{
	int	i;

	if (!sublexer)
		return ;
	if (sublexer->tokens)
	{
		i = 0;
		while (i < sublexer->token_count)
		{
			if (sublexer->tokens[i].text)
				free(sublexer->tokens[i].text);
			i++;
		}
		free(sublexer->tokens);
	}
	free(sublexer);
}
