/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_handlers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 21:56:14 by dopereir          #+#    #+#             */
/*   Updated: 2025/09/09 14:05:03 by dopereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*cmd_type_str(t_token_type type)
{
	if (type == T_WORD)
		return ("T_WORD");
	else if (type == T_PIPE)
		return ("|");
	else if (type == T_REDIR_IN)
		return ("<");
	else if (type == T_REDIR_OUT)
		return (">");
	else if (type == T_REDIR_APPEND)
		return (">>");
	else if (type == T_REDIR_HEREDOC)
		return ("<<");
	else if (type == T_AND)
		return ("&&");
	return ("UNKNOWN TYPE");
}

void	argument_redirs_error(t_token_type type)
{
	printf("minishell: syntax error near unexpected token `%s'\n",
		cmd_type_str(type));
}

int	partial_exec_err_free(t_parse_data *pd, t_exec_data *ctx)
{
	free_parsed_data(pd);
	free_lexer_tokens(ctx->lexer_ref);
	return (1);
}
