/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nayara <nayara@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/07 14:37:35 by nayara            #+#    #+#             */
/*   Updated: 2025/06/07 14:37:37 by nayara           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "minishell.h"

t_command	*parse_function(t_lexer *lexer)
{
	if (has_pipes(lexer))
		return parse_pipe();
	if (has_logical_operators())
		return parse_sequence();
	else
		return parse_simple_command();
}

t_command	*parse_simple_command(t_lexer *lexer)
{
	t_command	*cmd;
	int	args;
	int	i;
	int	arg_index;

	i = 0;
	arg_index = 0;
	cmd = malloc(sizeof(t_command));
	if (!cmd)
		return NULL;
	cmd->type = lexer->tokens[i].type; // dentro do loop
	args = count_args(lexer); // contar quantos tokens representam palavras e nao redir.
	cmd->argv = malloc(sizeof(char *) * (args + 1)); // array para armaz args do comando
	while (i < lexer->token_count)
	{
		if (lexer->tokens[i].type == T_WORD)
		{
			cmd->argv[arg_index] = strdup(lexer->tokens[i].text);
			if (! cmd->argv[arg_index])
			{
				free(cmd);
				return NULL;
			}
		}
		else if (lexer->tokens[i].type == T_REDIR_IN || lexer->tokens[i].type == T_REDIR_IN)

		i++;
		arg_index++;
	}
	// primeiro \token = comando
	// proximos tokens = argumentos
	// tratar redirecionamento
}

*parse_pipeline()
{
	separar \comandos por pipes
	cada comando vira um simple_command
	concetar comandos na pipeline
}