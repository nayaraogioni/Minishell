/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_helper_2.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 16:46:52 by nayara            #+#    #+#             */
/*   Updated: 2025/08/27 21:20:39 by dopereir         ###   ########.fr       */
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
		printf("TOKEN: <%s>	TYPE: <%d> EXIT_STATUS: <%d>\n",
			lexer->tokens[i].text, lexer->tokens[i].type, lexer->exit_status);
		i++;
	}
	printf("Numeber of tokens: %d\n", lexer->token_count);
}

void	clear_token(t_token *tokens, int token_count)
{
	int	i;

	i = 0;
	if (!tokens)
		return ;
	while (i < token_count)
	{
		if (tokens[i].text)
			free(tokens[i].text);
		i++;
	}
	free(tokens);
}

bool	bool_changer(bool key)
{
	return (!key);
}

t_token_type	determine_type_helper(char *token_text)
{
	if (ft_strcmp(token_text, "|") == 0)
		return (T_PIPE);
	else if (ft_strcmp(token_text, "<") == 0)
		return (T_REDIR_IN);
	else if (ft_strcmp(token_text, ">") == 0)
		return (T_REDIR_OUT);
	else if (ft_strcmp(token_text, ">>") == 0)
		return (T_REDIR_APPEND);
	else if (ft_strcmp(token_text, "<<") == 0)
		return (T_REDIR_HEREDOC);
	else if (ft_strcmp(token_text, "&&") == 0)
		return (T_AND);
	return (T_WORD);
}
