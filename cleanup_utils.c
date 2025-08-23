/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/23 19:04:00 by dopereir          #+#    #+#             */
/*   Updated: 2025/08/23 19:04:10 by dopereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	cleanup_iter(t_lexer *lexer, t_parse_data *pd)
{
	int	i;

	if (pd)
		free_parsed_data(pd);
	if (lexer)
	{
		if (lexer->tokens)
		{
			i = 0;
			while (i < lexer->token_count)
			{
				free (lexer->tokens[i].text);
				lexer->tokens[i].text = NULL;
				i++;
			}
			free (lexer->tokens);
			lexer->tokens = NULL;
			lexer->token_count = 0;
			lexer->input = NULL;
		}
	}
}
