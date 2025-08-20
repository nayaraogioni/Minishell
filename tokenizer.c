/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nayara <nayara@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 09:28:33 by dopereir          #+#    #+#             */
/*   Updated: 2025/08/20 17:07:57 by nayara           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "libft/libft.h"
#include "minishell.h"
#include <stddef.h>
#include <time.h>

bool	bool_changer(bool key)
{
	return (!key);
}

// func para validar se todas as aspas estão fechadas
int	validate_quotes(char *str)
{
	bool in_single_quote;
	bool in_double_quote;
	char *s;

	in_single_quote = false;
	in_double_quote = false;
	s = str;

	while (*s)
	{
		if (*s == '\'' && !in_double_quote)
			in_single_quote = bool_changer(in_single_quote);
		else if (*s == '"' && !in_single_quote)
			in_double_quote = bool_changer(in_double_quote);
		s++;
	}	if (in_single_quote)
	{
		printf("minishell: syntax error: unclosed single quote\n");
		return (0);
	}
	if (in_double_quote)
	{
		printf("minishell: syntax error: unclosed double quote\n");
		return (0);
	}
	return (1); // tds as aspas estão fechadas
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

int token_counter(char *str, char delim)
{
	char	*s;
	int	token_count_res;
	char	quote_char;

	s = str;
	token_count_res = 0;
	while (*s) // loop dentro da string de input
	{
		while (*s && *s == delim) // ingora espaços
			s++;
		if (*s == '\0') // se chegou ao fim quebra
			break;
		token_count_res++; // encontrou o inicio de um token
		if (*s == '"' || *s == '\'') // se for uma quote
		{
			quote_char = *s; // sinaliza onde esta na string a abertura da quote
			s++;
			while (*s && *s != quote_char) // itera sobre a string dentro da quote
				s++;
			if (*s == quote_char) // itera sobre a quote de fechamento
				s++;
		}
		else if ((*s && *(s + 1)) && ((*s == '>' && *(s + 1) == '>') ||
			(*s == '<' && *(s + 1) == '<') || (*s == '&' && *(s + 1) == '&'))) // verifica se é token duplo >> << &&
			s += 2;
		else if (*s == '|' || *s == '>' || *s == '<') // verifica se é token unico
			s++;
		else
		{
			 while (*s && *s != delim && *s != '|' && *s != '>' && *s != '<' && *s != '&') // itera enquanto nao for token ate chegar no proximo
			 	s++;
		}
	}
	return (token_count_res); // numero de tokens na string
}


t_token_type	determine_type(char *token_text, int qt_flag)
{
	int	i;
	if (!token_text || !*token_text)
		return (T_WORD);
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
	if (qt_flag == 1)
		return (T_WORD);
	i = 0;
	while (token_text[i])
	{
		if (token_text[i] == '$')
			return (T_VAR);
		i++;
	}
	return (T_WORD);
}

