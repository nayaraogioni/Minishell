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
# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>

// tokens we currently support
typedef enum e_token_type
{
	T_WORD,
	T_PIPE,
	T_REDIR_IN,
	T_REDIR_OUT,
	T_REDIR_APPEND,
	T_REDIR_HEREDOC,
	T_AND,
	T_VAR,
	T_WILDCARD,
	T_EOL
}		t_token_type;

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
	pid_t	last_bg_pid;
}			t_lexer;

#endif
