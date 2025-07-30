/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nayara <nayara@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 22:57:27 by dopereir          #+#    #+#             */
/*   Updated: 2025/07/08 14:35:11 by nayara           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#ifndef LEXER_H
# define LEXER_H
# define MAX_ARGS 1024

// tokens we currently support
typedef enum e_token_type
{
	T_WORD,			// 0 simple word
	T_PIPE,			// 1 |
	T_REDIR_IN,		// 2 <
	T_REDIR_OUT,	// 3 >
	T_REDIR_APPEND,	// 4 >>
	T_REDIR_HEREDOC,	// 5 << for here documents
	T_AND,	// 6 &&
	T_VAR,	// 7 $
	T_WILDCARD,		// 8 *.c for wildcards
	T_EOL			// 9 END OF LINE
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
	int		exit_status; // status do ultimo comando executado
	pid_t	last_bg_pid;  // PID do ultimo processo em background (para casos de fork ou sleep por ex)
}			t_lexer;

#endif
