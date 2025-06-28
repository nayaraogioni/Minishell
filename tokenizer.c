/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 09:28:33 by dopereir          #+#    #+#             */
/*   Updated: 2025/06/25 22:59:50 by dopereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
	}
	// se alguma aspas ainda está aberta retorna erro
	if (in_single_quote)
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
	else if (token_text[0] == '$' && ft_strlen(token_text) > 1)
		return (T_VAR);
	else if (token_text[0] == '*')
		return (T_WILDCARD);
	else
		return (T_WORD);
}

void	remove_quotes_from_token(t_token *token)
{
	char	*text;
	int	len;
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

int	add_token(t_token **tokens, int index, char *start, int len)
{
	(*tokens)[index].text = malloc(len + 1);
	if (!(*tokens)[index].text)
	{
		for (int j = 0; j < index; j++)
				free((*tokens)[j].text);
			free(*tokens);
			*tokens = NULL;
			return (-1);
	}
	ft_strncpy((*tokens)[index].text, start, len);
	(*tokens)[index].text[len] = '\0';
	remove_quotes_from_token(&(*tokens)[index]);
	(*tokens)[index].type = determine_type((*tokens)[index].text);
	return (0);
}

t_token	*split_tokens(char *str, char delim, t_lexer *lexer)
{
	char	*s;
	char	*start;
	t_token	*tokens;
	int		len;
	int		i;
	char	quote_char;

	s = str;
	quote_char = '\0';
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
		if (*s == '$' && *(s + 1) != delim)
		{
			start = s;
			s++;
			while (*s && (ft_isalnum(*s) || *s == '_'))
				s++;
			len = s - start;
		}
		else if (*s == '"' || *s == '\'')
		{
			quote_char = *s;
			s++;
			while (*s && *s != quote_char)
				s++;
			if (*s == quote_char)
				s++;
			len = s - start;
		}
		else if ((*s && *(s + 1)) && ((*s == '>' && *(s + 1) == '>') ||
			(*s == '<' && *(s + 1) == '<') || (*s == '&' && *(s + 1) == '&')))
		{
			len = 2;
			s += 2;
		}
		else if (*s == '|' || *s == '>' || *s == '<' || *s == '$')
		{
			len = 1;
			s++;
		}
		else
		{
			while (*s && *s != delim && *s != '|' && *s != '>' && *s != '<' && *s != '&')
				s++;
			len = s - start;
		}
		add_token(&tokens, i, start, len);
		i++;
	}
	lexer->tokens = tokens;
	return (tokens);
}

void	lexing_input(t_lexer *lexer, char delim)
{
	if (!validate_quotes(lexer->input)) // validade_quotes retorna 1 se bem sucedida
	{
		lexer->token_count = 0;
		lexer->tokens = NULL;
		return ;
	}
	if (lexer->tokens && lexer->token_count > 0)
	{
		clear_token(lexer->tokens, lexer->token_count);
		lexer->tokens = NULL;
	}
	lexer->token_count = token_counter(lexer->input, delim);
	lexer->tokens = split_tokens(lexer->input, delim, lexer);
}

//function to print tokens for debugging
void	print_tokens(t_lexer *lexer)
{
	for (int i = 0; i < lexer->token_count; i++)
	{
		printf("TOKEN: <%s>	TYPE: <%d>\n", lexer->tokens[i].text, lexer->tokens[i].type);
	}
	printf("Numeber of tokens: %d\n", lexer->token_count);
}
