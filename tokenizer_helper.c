/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_helper.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nayara <nayara@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 16:30:41 by nayara            #+#    #+#             */
/*   Updated: 2025/08/20 17:07:24 by nayara           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	remove_quotes_from_token(t_token *token)
{
	char	*text;
	int		len;
	char	*new_text;

	text = token->text;
	len = ft_strlen(text);
	if (len >= 2 && ((text[0] == '"' && text[len - 1] == '"') ||
		(text[0] == '\'' && text[len - 1] == '\'')))
	{
		new_text = malloc(sizeof(char) * (len - 1));
		if (!new_text)
			return ;
		ft_strncpy(new_text, text + 1, len - 2);
		new_text[len - 2] = '\0';
		free(text);
		token->text = new_text;
	}
}

int	add_token(t_token **tokens, int index, char *start, int len, int qt_flag, int join_prev)
{
	char	*txt;
	int j;

	txt = malloc(len + 1);
	if (!txt)
	{
		j = 0;
		while (j < index)
		{
			free((*tokens)[j].text);
			j++;
		}
		free(*tokens);
		*tokens = NULL;
		return (-1);
	}
	ft_strncpy(txt, start, len);
	txt[len] = '\0';
	(*tokens)[index].text = txt;
	(*tokens)[index].quot = qt_flag;
	(*tokens)[index].join_prev = join_prev;
	//remove_quotes_from_token(&(*tokens)[index]);
	(*tokens)[index].type = determine_type((*tokens)[index].text, qt_flag);//
	return (0);
}

void	free_tokens_partial(t_token *tokens, int count)
{
	int	j;

	j = 0;
	while (j < count)
	{
		free (tokens[j].text);
		j++;
	}
	free (tokens);
}

void	lexing_input(t_lexer *lexer, char delim)
{
	if (lexer->tokens && lexer->token_count > 0)
	{
		clear_token(lexer->tokens, lexer->token_count);
		lexer->tokens = NULL;
		lexer->token_count = 0;
	}
	if (!validate_quotes(lexer->input)) // validade_quotes retorna 1 se bem sucedida
	{
		lexer->tokens = NULL;
		lexer->token_count = 0;
		return ;
	}
	lexer->token_count = token_counter(lexer->input, delim);
	if (!split_tokens(lexer->input, delim, lexer))
	{
		lexer->tokens = NULL;
		lexer->token_count = 0;
		return ;
	}
}

char	*join_words(char *a, char *b)
{
	size_t	la;
	size_t	lb;
	char	*res;

	if (!a && !b)
		return (NULL);
	la = a ? ft_strlen(a) : 0;
	lb = b ? ft_strlen(b) : 0;
	res = malloc(la + lb + 1);
	if (!res)
		return (NULL);
	if (a)
		ft_memcpy(res, a, la);
	if (b)
		ft_memcpy(res + la, b, lb);
	res[la + lb] = '\0';
	if (a)
		free(a);
	return (res);
}