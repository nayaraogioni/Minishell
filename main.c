/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 23:15:37 by dopereir          #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2025/08/14 09:47:25 by dopereir         ###   ########.fr       */
=======
/*   Updated: 2025/07/19 02:08:59 by dopereir         ###   ########.fr       */
>>>>>>> 2eb9026dfc52170b8260ec677da6b87387b1c6f7
/*                                                                            */
/* ************************************************************************** */

#include "libft/libft.h"
#include "minishell.h"
#include <readline/history.h>
<<<<<<< HEAD
=======
#include <signal.h>
>>>>>>> 2eb9026dfc52170b8260ec677da6b87387b1c6f7

//-g -fsanitize=address to use on gdb when needed

static void	cleanup_iter(t_lexer *lexer, t_parse_data *pd)
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
			//free (lexer->input);
			lexer->input = NULL;
		}
	}
}

<<<<<<< HEAD
=======
// Função para testar comandos específicos
void test_specific_commands(t_env *my_env)
{
	printf("\n=== TESTANDO COMANDOS ESPECÍFICOS ===\n");

	char *test_commands[] = {
		"ls -la",                           // Comando simples
		"cat file.txt",                     // Comando simples com arquivo
		"ls > output.txt",                  // Redirecionamento saída
		"cat < input.txt",                  // Redirecionamento entrada
		"echo hello >> log.txt",            // Append
		"cat << EOF",                       // Heredoc
		"ls | grep test",                   // Pipeline simples
		"ls -la | grep txt | wc -l",        // Pipeline múltiplo
		"mkdir test && cd test",            // Sequência simples
		"ls && echo done && pwd",           // Sequência múltipla
		"ls | grep test && echo found",     // Pipeline + sequência
		"mkdir dir && ls | grep dir",       // Sequência + pipeline
		"cat < input.txt | grep test > output.txt", // Pipeline com redirecionamentos
		NULL
	};

	for (int i = 0; test_commands[i]; i++)
	{
		printf("\n--- Testando: '%s' ---\n", test_commands[i]);

		t_lexer *lexer = malloc(sizeof(t_lexer));
		lexer->input = ft_strdup(test_commands[i]);
		lexer->tokens = NULL;
		lexer->token_count = 0;

		// Tokenização
		lexing_input(lexer, ' ');

		printf("Tokens:\n");
		print_tokens(lexer);

		// Parsing
		t_command *cmd = parse_function(lexer, my_env);
		if (cmd)
		{
			printf("Parse Tree:\n");
			print_command_tree(cmd, 0);
			free_command(cmd);
		}
		else
		{
			printf("ERRO: Falha no parsing!\n");
		}

		// Limpeza
		free(lexer->input);
		for (int j = 0; j < lexer->token_count; j++)
			free(lexer->tokens[j].text);
		free(lexer->tokens);
		free(lexer);

		printf("----------------------------------------\n");
	}
}

// Função para validar se o parsing está correto
int validate_parsing(t_env *my_env)
{
	printf("\n=== VALIDAÇÃO DE PARSING ===\n");
	int errors = 0;

	// Teste 1: Comando simples
	{
		t_lexer *lexer = malloc(sizeof(t_lexer));
		lexer->input = ft_strdup("ls -la");
		lexer->tokens = NULL;
		lexer->token_count = 0;

		lexing_input(lexer, ' ');
		t_command *cmd = parse_function(lexer, my_env);

		if (!cmd || cmd->type != T_WORD || !cmd->name || strcmp(cmd->name, "ls") != 0)
		{
			printf("❌ ERRO: Comando simples falhou\n");
			errors++;
		}
		else
		{
			printf("✅ OK: Comando simples\n");
		}

		if (cmd) free_command(cmd);
		free(lexer->input);
		for (int j = 0; j < lexer->token_count; j++)
			free(lexer->tokens[j].text);
		free(lexer->tokens);
		free(lexer);
	}

	// Teste 2: Pipeline
	{
		t_lexer *lexer = malloc(sizeof(t_lexer));
		lexer->input = ft_strdup("ls | grep test");
		lexer->tokens = NULL;
		lexer->token_count = 0;

		lexing_input(lexer, ' ');
		t_command *cmd = parse_function(lexer, my_env);

		if (!cmd || cmd->type != T_PIPE || cmd->command_count != 2)
		{
			printf("❌ ERRO: Pipeline falhou\n");
			errors++;
		}
		else
		{
			printf("✅ OK: Pipeline\n");
		}

		if (cmd) free_command(cmd);
		free(lexer->input);
		for (int j = 0; j < lexer->token_count; j++)
			free(lexer->tokens[j].text);
		free(lexer->tokens);
		free(lexer);
	}

	// Teste 3: Sequência
	{
		t_lexer *lexer = malloc(sizeof(t_lexer));
		lexer->input = ft_strdup("mkdir test && cd test");
		lexer->tokens = NULL;
		lexer->token_count = 0;

		lexing_input(lexer, ' ');
		t_command *cmd = parse_function(lexer, my_env);

		if (!cmd || cmd->type != T_AND || cmd->command_count != 2)
		{
			printf("❌ ERRO: Sequência falhou\n");
			errors++;
		}
		else
		{
			printf("✅ OK: Sequência\n");
		}

		if (cmd) free_command(cmd);
		free(lexer->input);
		for (int j = 0; j < lexer->token_count; j++)
			free(lexer->tokens[j].text);
		free(lexer->tokens);
		free(lexer);
	}

	printf("\nResultado: %d erros encontrados\n", errors);
	return errors;
}

// Main atualizada
>>>>>>> 2eb9026dfc52170b8260ec677da6b87387b1c6f7
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

	my_env = NULL;
	env_init(&my_env, envp);
	printf("count_env result after env_init: <%d>\n", count_env(my_env));
	while (1)
	{
		input = readline("MINISHELL>$ ");

		if (!input) //CTRL-D EOF
<<<<<<< HEAD
			break;
		if (ft_strcmp(input, "exit") == 0)
		{
			free(input);
			break;
		}
=======
			break;
		if (ft_getenv(my_env, "?"))
			lexer->exit_status = ft_atoi(ft_getenv(my_env, "?"));
		else
			lexer->exit_status = 0;
>>>>>>> 2eb9026dfc52170b8260ec677da6b87387b1c6f7
		if (ft_strlen(input) == 0)
		{
			free(input);
			continue;
		}

<<<<<<< HEAD
=======
		// Comandos especiais
		if (ft_strcmp(input, "exit") == 0)
		{
			free(input);
			break;
		}
>>>>>>> 2eb9026dfc52170b8260ec677da6b87387b1c6f7
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
<<<<<<< HEAD
			cleanup_iter(lexer, &pd);
			free (input);
			//free_parsed_data(&pd);
			//free(input);
=======
			free_parsed_data(&pd);
			free(input);
>>>>>>> 2eb9026dfc52170b8260ec677da6b87387b1c6f7
			continue ;
		}

		exec_parsed_cmds(&pd, &my_env);
		add_history(input);

		if (ft_getenv(my_env, "?"))
			lexer->exit_status = ft_atoi(ft_getenv(my_env, "?"));
		else
			lexer->exit_status = 0;

		cleanup_iter(lexer, &pd);
		free (input);
		// Libera tokens
		/*for (int i = 0; i < lexer->token_count; i++)
		{
			if (lexer->tokens[i].text)
				free(lexer->tokens[i].text);
		}
		if (lexer->tokens)
			free(lexer->tokens);
		//main_exit_status = pd.pd_exit_status;
		//lexer->exit_status = main_exit_status;
		free_parsed_data(&pd);
<<<<<<< HEAD
		free(input);*/
=======
		free(input);
>>>>>>> 2eb9026dfc52170b8260ec677da6b87387b1c6f7
		//printf("\n=======================END OF CMD===========================\n");
	}

	free(lexer);
	printf("count_env result before clean: <%d>\n", count_env(my_env));
	clean_env_list(&my_env);
<<<<<<< HEAD
	printf("count_env result after clean: <%d>\n", count_env(my_env));
=======
>>>>>>> 2eb9026dfc52170b8260ec677da6b87387b1c6f7
	return (0);
}
