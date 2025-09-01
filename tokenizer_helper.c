/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_helper.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 16:30:41 by nayara            #+#    #+#             */
/*   Updated: 2025/08/27 22:02:05 by dopereir         ###   ########.fr       */
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
	if (len >= 2 && ((text[0] == '"' && text[len - 1] == '"')
			|| (text[0] == '\'' && text[len - 1] == '\'')))
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

int	add_token(t_token **tokens, int index, t_proc_token *pt)
{
	char	*txt;
	int		j;

	txt = malloc(pt->len + 1);
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
	ft_strncpy(txt, pt->start, pt->len);
	txt[pt->len] = '\0';
	(*tokens)[index].text = txt;
	(*tokens)[index].quot = pt->qt_flag;
	(*tokens)[index].join_prev = pt->join_prev;
	(*tokens)[index].type = determine_type((*tokens)[index].text, pt->qt_flag);
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

void	lexing_input(t_lexer *lexer, char delim)//starting point of the issue
{
	if (lexer->tokens && lexer->token_count > 0)
	{
		clear_token(lexer->tokens, lexer->token_count);
		lexer->tokens = NULL;
		lexer->token_count = 0;
	}
	if (!validate_quotes(lexer->input))
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
	la = 0;
	lb = 0;
	if (a)
		la = ft_strlen(a);
	if (b)
		lb = ft_strlen(b);
	res = malloc(la + lb + 1);
	if (!res)
		return (NULL);
	ft_memcpy(res, a, la);
	ft_memcpy(res + la, b, lb);
	res[la + lb] = '\0';
	if (a)
		free(a);
	return (res);
}
