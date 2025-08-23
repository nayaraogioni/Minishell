/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 23:15:37 by dopereir          #+#    #+#             */
/*   Updated: 2025/08/23 17:06:08 by dopereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/libft.h"
#include "minishell.h"
#include <readline/history.h>
#include <stdlib.h>
#include <time.h>

//-g -fsanitize=address to use on gdb when needed

void	cleanup_iter(t_lexer *lexer, t_parse_data *pd)
{
	int	i;

	if (pd)
		free_parsed_data(pd);
	if (lexer)
	{
		if (lexer->tokens)
		{
			i = 0;
			while (i < lexer->token_count)
			{
				free (lexer->tokens[i].text);
				lexer->tokens[i].text = NULL;
				i++;
			}
			free (lexer->tokens);
			lexer->tokens = NULL;
			lexer->token_count = 0;
			lexer->input = NULL;
		}
	}
}

int main(int argc, char **argv, char **envp)
{
	t_lexer			*lexer;
	t_parse_data	pd;
	t_env			*my_env;
	char			*input;
	struct sigaction	sa_int = {0}, sa_quit = {0};
	//static	int		main_exit_status = 0;

	sa_int.sa_handler = sigint_handler;
	sigemptyset(&sa_int.sa_mask);
	sa_int.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sa_int, NULL);

	sa_quit.sa_handler = SIG_IGN;//instead of sigquit_handler;
	sigemptyset(&sa_quit.sa_mask);
	sa_quit.sa_flags = 0;
	sigaction(SIGQUIT, &sa_quit, NULL);

	(void)argc;
	(void)argv;

	lexer = malloc(sizeof(t_lexer));
	lexer->input = NULL;
	lexer->tokens = NULL;
	lexer->token_count = 0;
	lexer->exit_status = 0;

	if (env_init(&my_env, envp) != 0)
	{
		free (lexer);
		return (0);
	}
	while (1)
	{
		input = readline("MINISHELL>$ ");

		if (!input)
			break;
		if (ft_exit(input))
			break ;
		if (ft_strlen(input) == 0)
		{
			free(input);
			continue;
		}

		lexer->input = input;
		lexer->tokens = NULL;
		lexer->token_count = 0;

		//printf("\n--- LEXING ---\n");
		lexing_input(lexer, ' ');
		//print_tokens(lexer);

		//printf("\n--- PARSING ---\n");
		pd = format_parsed_data(lexer, my_env);
		//print_parsed_data(&pd);
		if (handle_all_heredocs(&pd, my_env) < 0)
		{
			add_history(input);
			cleanup_iter(lexer, &pd);
			free (input);
			//free_parsed_data(&pd);
			//free(input);
			continue ;
		}

		exec_parsed_cmds(&pd, &my_env, lexer);
		add_history(input);

		if (ft_getenv(my_env, "?"))
			lexer->exit_status = ft_atoi(ft_getenv(my_env, "?"));
		else
			lexer->exit_status = 0;

		cleanup_iter(lexer, &pd);
		free (input);
		//printf("\n=======================END OF CMD===========================\n");
	}

	free(lexer);
	clean_env_list(&my_env);
	return (0);
}
