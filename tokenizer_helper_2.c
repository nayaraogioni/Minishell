/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_helper_2.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 16:46:52 by nayara            #+#    #+#             */
/*   Updated: 2025/08/22 12:57:08 by dopereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_wordish(t_token *t)
{
	return (t->type == T_WORD || t->type == T_VAR);
}

//function to print tokens for debugging
void	print_tokens(t_lexer *lexer)
{
	int	i;

	i = 0;
	while (i < lexer->token_count)
	{
		printf("TOKEN: <%s>	TYPE: <%d> EXIT_STATUS: <%d>\n", \
			lexer->tokens[i].text, lexer->tokens[i].type, lexer->exit_status);
		i++;
	}
	printf("Numeber of tokens: %d\n", lexer->token_count);
}
