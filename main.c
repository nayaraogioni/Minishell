/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 23:15:37 by dopereir          #+#    #+#             */
/*   Updated: 2025/06/04 21:09:21 by dopereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//handle ctrl-D later
// ctrl-c (sigint)
// ctrl-d (EOF signal)

int	main(void)
{
	t_lexer	*lexer;

	lexer = malloc(sizeof(t_lexer));
	lexer->input = NULL;
	lexer->tokens = NULL;
	lexer->path = NULL;
	lexer->token_count = 0;

	int	flag = 1;

	while(flag)
	{
		lexer->input = readline("PROMPT>$ "); // READ (1/2)
		if (lexer->input)
		{
			lexing_input(lexer, ' '); // READ(2/2)
			print_tokens(lexer); // PRINT TOKENS
			execute(lexer);
			//parse_function(lexer);
				// criar t_parse_data -> 
			
			add_history(lexer->input);
			free (lexer->input);
		}
		//keepRunning = 0;
		printf("KEEP RUNNING: %d\n", keepRunning);
	}
	return (0);
}
