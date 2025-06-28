/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 22:57:27 by dopereir          #+#    #+#             */
/*   Updated: 2025/06/08 23:54:33 by dopereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
# define LEXER_H
# include "minishell.h"

// tokens we currently support
typedef enum e_token_type
{
	T_WORD,			// 0 simple word
	T_PIPE,			// 1 |
	T_REDIR_IN,		// 2 <
	T_REDIR_OUT,	// 3 >
	T_REDIR_APPEND,	// 4 >>
	T_REDIR_HEREDOC,	// 5 << for here documents
	T_AND,	// &&
	T_VAR,	// $
	T_WILDCARD,		// 7 *.c for wildcards
	T_EOL			// 8 END OF LINE
}			t_token_type;

typedef struct s_token
{
	t_token_type	type;
	char			*text;
}			t_token;

typedef struct s_lexer
{
	char	*input;
	t_token	*tokens;
	int		token_count;
	char	*path;
	char	*args[MAX_ARGS];
}			t_lexer;

#endif
