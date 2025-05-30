/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 09:28:33 by dopereir          #+#    #+#             */
/*   Updated: 2025/05/30 16:55:25 by dopereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	clear_token(t_token *tokens, int token_count)
{
	int	i;

	i = 0;
	if (!tokens)
		return ;
	while (i < token_count)
	{
		free(tokens[i].text);
		i++;
	}
	free(tokens);
}

int	token_counter(char *str, char delim)
{
	int	token_count_res;
	int	i;

	token_count_res = 0;
	i = 0;
	while (str[i])
	{
		while (str[i] && str[i] == delim)
			i++;
		if (str[i] == '\0')
			break ;
		token_count_res++;
		while (str[i] && str[i] != delim)
			i++;
	}
	return (token_count_res);
}

t_token_type	determine_type(char *token_text)
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
	else if (token_text[0] == '*')
		return (T_WILDCARD);
	else
		return (T_WORD);
}

t_token	*split_tokens(char *str, char delim, t_lexer *lexer)
{
	char	*s;
	char	*start;
	t_token	*tokens;
	int		len;
	int		i;

	s = str;
	tokens = malloc(lexer->token_count * sizeof(t_token));
	if (!tokens)
		return (NULL);
	i = 0;
	while (*s && i < lexer->token_count)
	{
		while (*s && *s == delim)
			s++;
		if (*s == '\0')
			break ;
		start = s;
		while (*s && *s != delim)
			s++;
		len = s - start;
		tokens[i].text = malloc(len + 1);
		if (!tokens[i].text)
		{
			for (int j = 0; j < i; j++)
				free(tokens[j].text);
			free(tokens);
			return (NULL);
		}
		ft_strncpy(tokens[i].text, start, len);
		tokens[i].text[len] = '\0';
		tokens[i].type = determine_type(tokens[i].text);
		i++;
	}
	lexer->tokens = tokens;
	return (tokens);
}

void	lexing_input(t_lexer *lexer, char delim)
{
	clear_token(lexer->tokens, lexer->token_count);
	lexer->token_count = token_counter(lexer->input, delim);
	lexer->tokens = split_tokens(lexer->input, delim, lexer);
}

//function to print tokens for debugging
void	print_tokens(t_lexer *lexer)
{
	for (int i = 0; i < lexer->token_count; i++)
	{
		printf("TOKEN: <%s>		TYPE: %d\n", lexer->tokens[i].text, lexer->tokens[i].type);
	}
}

