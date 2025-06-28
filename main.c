/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 23:15:37 by dopereir          #+#    #+#             */
/*   Updated: 2025/06/25 23:54:47 by dopereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//handle ctrl-D later
// ctrl-c (sigint)
// ctrl-d (EOF signal)

/*void	copy_arguments(char *dst[MAX_ARGS], char *src[MAX_ARGS])
{
	int	i;

	i = 0;
	while (src[i] && i < MAX_ARGS - 1)
	{
		dst[i] = src[i];
		i++;
	}
	dst[i] = NULL; // Ensure the last element is NULL
}

void	initialize_command(t_token	*tokens, t_command *command)
{
	int	i;
	char	full_path[1024];

	i = 0;
	if (check_command_exists(tokens->text, tokens->type, full_path))
	{
		command->name = tokens->text;
	}
	else
	{
		printf("Command not found: %s\n", lexer->tokens[0].text);
		exit(EXIT_FAILURE);
	}
	//command->args = copy_arguments(command->args, lexer->args);
}

t_parse_data	*testing_create_cmd_list(t_lexer *lexer)
{
	t_parse_data	*parse_data;
	parse_data = malloc(sizeof(t_parse_data));
	if (!parse_data)
		return (NULL);
	parse_data->n_cmds = 0;
	for (int i = 0; i < lexer->token_count; i++) //initialize to null each commands
	{
		parse_data->data[i] = NULL;
	}
	for (int i = 0; i < lexer->token_count; i++)
	{
		initialize_command(lexer->tokens[i], parse_data->data[i]);
	}

	return (parse_data);
	}*/


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
			// execute t_parse_data ->
			add_history(lexer->input);
			free (lexer->input);
		}
		//keepRunning = 0;
		//printf("KEEP RUNNING: %d\n", keepRunning);
	}
	return (0);
}
