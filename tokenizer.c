/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nayara <nayara@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 09:28:33 by dopereir          #+#    #+#             */
/*   Updated: 2025/07/08 14:24:25 by nayara           ###   ########.fr       */
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

static void	free_tokens_partial(t_token *tokens, int count)
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

t_token	*split_tokens(char *str, char delim, t_lexer *lexer)
{
	char	*s;
	char	*start;
	char	*tok_begin;
	t_token	*tokens;
	int		len;
	int		i;
	char	quote_char;
	int		rc;
	int		qt_flag;
	int		join_prev;

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
<<<<<<< HEAD
		tok_begin = s;
		qt_flag = 0;
		join_prev = (tok_begin > str && *(tok_begin - 1) != delim) ? 1 : 0;
=======
		start = s;
>>>>>>> 0384cda6b8b7f354a1d6b8ffa81772123f696e80
		if (*s == '$' && *(s + 1))
		{
			start = s;
			s++;
			while (*s && (ft_isalnum(*s) || *s == '_' || *s == '?' || *s == '!' || *s == '@' || *s == '#' || *s == '$'))
				s++;
			len = s - start;
		}
		else if (*s == '"' || *s == '\'') //this lexes a quoted piece?
		{
			quote_char = *s;
			qt_flag = (quote_char == '\'') ? 1 : 2;
			s++;
			start = s;
			if (quote_char == '"')
			{
				// In double quotes: a backslash escapes ", \, $, `, and newline
				while (*s)
				{
					if (*s == '\\' && s[1] != '\0')
					{
						// Skip over the escaped char so we don't mistake \" for the end
						s += 2;
						continue;
					}
					if (*s == '"')
						break;
					s++;
				}
			}
			else
			{
				// In single quotes: everything until next single quote
				while (*s && *s != quote_char)
					s++;
			}
			len = s - start;
			if (*s == quote_char)
				s++;
		}
		else if ((*s && *(s + 1)) && ((*s == '>' && *(s + 1) == '>') ||
			(*s == '<' && *(s + 1) == '<') || (*s == '&' && *(s + 1) == '&')))
		{
			start = s;
			len = 2;
			s += 2;
		}
		else if (*s == '|' || *s == '>' || *s == '<' || *s == '$')
		{
			start = s;
			len = 1;
			s++;
		}
		else
		{
			start = s;
			while (*s && *s != delim && *s != '|' && *s != '>' && *s != '<' && *s != '&')
				s++;
			len = s - start;
		}
		rc = add_token(&tokens, i, start, len, qt_flag, join_prev);//
		if (rc < 0)
		{
			free_tokens_partial(tokens, i);
			lexer->tokens = NULL;
			lexer->token_count = 0;
			return (NULL);
		}
		i++;
	}
	lexer->token_count = i;
	lexer->tokens = tokens;
	return (tokens);
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
