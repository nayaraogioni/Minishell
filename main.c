/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nayara <nayara@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 23:15:37 by dopereir          #+#    #+#             */
/*   Updated: 2025/08/23 13:12:16 by nayara           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/libft.h"
#include "minishell.h"
#include <readline/history.h>
#include <stdlib.h>
#include <time.h>

int	main_loop_helper(char *input, t_lexer *lexer)
{
	if (!input)
		return (2);
	if (ft_exit(input) == 1)
		return (2);
	if (ft_strlen(input) == 0)
	{
		free(input);
		return (1);
	}
	lexer->input = input;
	lexer->tokens = NULL;
	lexer->token_count = 0;
	lexing_input(lexer, ' ');
	return (0);
}

///NOTE: return 2 to indicate break ;
///NOTE: return 1 to indicate continue ;
///NOTE: return 0 to indicate sucess, loop runs again
int	main_loop(t_env *my_env, t_lexer *lexer, t_parse_data *pd)
{
	char	*input;
	int		rc;

	input = readline("MINISHELL>$ ");
	rc = main_loop_helper(input, lexer);
	if (rc != 0)
		return (rc);
	*pd = format_parsed_data(lexer, my_env);
	if (handle_all_heredocs(pd, my_env) < 0)
	{
		add_history(input);
		cleanup_iter(lexer, pd);
		free (input);
		return (1);
	}
	exec_parsed_cmds(pd, &my_env, lexer);
	add_history(input);
	if (ft_getenv(my_env, "?"))
		lexer->exit_status = ft_atoi(ft_getenv(my_env, "?"));
	else
		lexer->exit_status = 0;
	cleanup_iter(lexer, pd);
	free (input);
	return (0);
}

static void	setup_init_signals(void)
{
	struct sigaction	sa_int;
	struct sigaction	sa_quit;

	memset(&sa_int, 0, sizeof(sa_int));
	memset(&sa_quit, 0, sizeof(sa_quit));
	sa_int.sa_handler = sigint_handler;
	sigemptyset(&sa_int.sa_mask);
	sa_int.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sa_int, NULL);
	sa_quit.sa_handler = SIG_IGN;
	sigemptyset(&sa_quit.sa_mask);
	sa_quit.sa_flags = 0;
	sigaction(SIGQUIT, &sa_quit, NULL);
}

//returns 0 on sucess
//return -1 on failure
int	init_lexer_and_env(t_lexer *lexer, t_env **my_env, char **envp)
{
	lexer->input = NULL;
	lexer->tokens = NULL;
	lexer->token_count = 0;
	lexer->exit_status = 0;
	if (env_init(my_env, envp) != 0)
		return (-1);
	return (0);
}

int	main(int argc, char **argv, char **envp)
{
	t_lexer				*lexer;
	t_parse_data		pd;
	t_env				*my_env;
	int					rc;

	(void)argc;
	(void)argv;
	setup_init_signals();
	lexer = malloc(sizeof(t_lexer));
	if (!lexer)
		return (1);
	if (init_lexer_and_env(lexer, &my_env, envp) != 0)
		return (free(lexer), 0);
	while (1)
	{
		rc = main_loop(my_env, lexer, &pd);
		if (rc == 2)
			break ;
		else if (rc == 1)
			continue ;
	}
	free(lexer);
	clean_env_list(&my_env);
	return (0);
}
