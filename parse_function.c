/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_function.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 19:54:26 by dopereir          #+#    #+#             */
/*   Updated: 2025/05/30 20:13:26 by dopereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_parse_data	*parse_function(t_lexer *lexer)
{
	//t_parse_data	*commands; #CONJUNTO DE COMANDOS

	//t_command command = NULL; #COMMANDO INDIVUAL

	command = malloc(sizeof(t_command) * 1);

	char *token = lexer->tokens[0]; //validar comando na pasta bin

	if (token == NULL || !is_valid_command(token))
	{
		free(command);
		//print error message
		//retornar ao prompt
		return (NULL); // Erro: comando inválido
	}
	command->name = strdup(token); // ls
	if (lexer->tokens[i].type == REDIRECT)
	{
		//lexer->token[2].
	}
	command->data->redirects = lexer->tokens[1].type 


}