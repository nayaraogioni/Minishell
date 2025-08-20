/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_helper_2.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nayara <nayara@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 16:46:52 by nayara            #+#    #+#             */
/*   Updated: 2025/08/20 16:47:07 by nayara           ###   ########.fr       */
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
	for (int i = 0; i < lexer->token_count; i++)
	{
		printf("TOKEN: <%s>	TYPE: <%d> EXIT_STATUS: <%d>\n", lexer->tokens[i].text, lexer->tokens[i].type, lexer->exit_status);
	}
	printf("Numeber of tokens: %d\n", lexer->token_count);
}