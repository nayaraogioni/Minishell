/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 23:15:37 by dopereir          #+#    #+#             */
/*   Updated: 2025/07/03 23:57:48 by dopereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//handle ctrl-D later
// ctrl-c (sigint)
// ctrl-d (EOF signal)


static const char *cmd_type_str(t_token_type type)
{
	switch (type) {
	case T_WORD:          return "T_WORD";
	case T_PIPE:          return "T_PIPE";
	case T_REDIR_IN:      return "T_REDIR_IN";
	case T_REDIR_OUT:     return "T_REDIR_OUT";
	case T_REDIR_APPEND:  return "T_REDIR_APPEND";
	case T_REDIR_HEREDOC: return "T_REDIR_HEREDOC";
	case T_AND:           return "T_AND";
	default:              return "UNKNOWN";
	}
}

static void indent(int depth)
{
	for (int i = 0; i < depth; i++)
		printf("  ");
}

//MAIN PARA TESTE DE PARSING
// Função para imprimir a árvore de comandos (debug)
void print_command_tree(t_command *cmd, int depth)
{
	int i;

	if (!cmd) {
		indent(depth);
		printf("[null command]\n");
		return;
	}

	// 1) Header: type, name, path, pid
	indent(depth);
	printf("COMMAND (@%d)\n", cmd->command_count);

	indent(depth); printf("  Type:            %s (%d)\n",
						  cmd_type_str(cmd->type), cmd->type);

	indent(depth); printf("  Name:            %s\n",
						  cmd->name ? cmd->name : "NULL");

	indent(depth); printf("  Path:            %s\n",
						  cmd->path ? cmd->path : "NULL");

	indent(depth); printf("  pid_filename_out:%d\n",
						  cmd->pid_filename_output);

	// 2) I/O redirections
	if (cmd->input_file || cmd->output_file || cmd->filename) {
		indent(depth); printf("  Redirections:\n");
		if (cmd->input_file) {
			indent(depth); printf("    <  %s\n", cmd->input_file);
		}
		if (cmd->output_file) {
			indent(depth); printf("    >  %s\n", cmd->output_file);
		}
		if (cmd->type == T_REDIR_APPEND && cmd->output_file) {
			indent(depth); printf("    >> %s\n", cmd->output_file);
		}
		if (cmd->type == T_REDIR_HEREDOC && cmd->hd_delim) {
			indent(depth); printf("    << %s\n", cmd->hd_delim);
		}
	}

	// 3) argv / args
	indent(depth);
	printf("  ARGS: [");
	for (i = 0; cmd->argv[i]; i++) {
		printf("\"%s\"", cmd->argv[i]);
		if (cmd->argv[i + 1])
			printf(", ");
	}
	printf("]\n");

	// 4) Recursively print sub-commands for PIPE or AND
	switch (cmd->type) {
	case T_PIPE:
	case T_AND:
		indent(depth);
		printf("  %s (%d sub-commands):\n",
			   cmd->type == T_PIPE ? "PIPELINE" : "SEQUENCE",
			   cmd->command_count);
		for (i = 0; i < cmd->command_count; i++) {
			indent(depth);
			printf("    Sub-command %d:\n", i + 1);
			print_command_tree(cmd->commands[i], depth + 2);
		}
		break;

	default:
		// Nothing else to do
		break;
	}
}

// Função para testar comandos específicos
void test_specific_commands(void)
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
		t_command *cmd = parse_function(lexer);
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
int validate_parsing(void)
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
		t_command *cmd = parse_function(lexer);

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
		t_command *cmd = parse_function(lexer);

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
		t_command *cmd = parse_function(lexer);

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
int main(void)
{
	t_lexer			*lexer;
	t_parse_data	pd;
	char			*input;
   // int mode = 0; // 0 = interativo, 1 = teste automático

	printf("=== MINISHELL PARSER TESTER ===\n");
	printf("Comandos especiais:\n");
	printf("  'test'     - Executa testes automáticos\n");
	printf("  'validate' - Executa validação de parsing\n");
	printf("  'exit'     - Sair do programa\n");
	printf("  'clear'    - Limpar tela\n");
	printf("================================\n\n");

	lexer = malloc(sizeof(t_lexer));
	lexer->input = NULL;
	lexer->tokens = NULL;
	lexer->token_count = 0;

	while (1)
	{
		input = readline("MINISHELL>$ ");

		if (!input)
		{
			printf("\nSaindo...\n");
			break;
		}

		if (strlen(input) == 0)
		{
			free(input);
			continue;
		}

		// Comandos especiais
		if (strcmp(input, "exit") == 0)
		{
			free(input);
			break;
		}
		else if (strcmp(input, "test") == 0)
		{
			free(input);
			test_specific_commands();
			continue;
		}
		else if (strcmp(input, "validate") == 0)
		{
			free(input);
			validate_parsing();
			continue;
		}
		else if (strcmp(input, "clear") == 0)
		{
			system("clear");
			free(input);
			continue;
		}

		// Processar comando normal
		lexer->input = input;
		lexer->tokens = NULL;
		lexer->token_count = 0;

		printf("\n--- LEXING ---\n");
		lexing_input(lexer, ' ');
		print_tokens(lexer);

		printf("\n--- PARSING ---\n");
		pd = format_parsed_data(lexer);
		print_parsed_data(&pd);
		exec_parsed_cmds(&pd); //
		//************************************
		add_history(input);

		// Libera tokens
		for (int i = 0; i < lexer->token_count; i++)
		{
			if (lexer->tokens[i].text)
				free(lexer->tokens[i].text);
		}
		if (lexer->tokens)
			free(lexer->tokens);

		free(input);

		printf("\n==================================================\n");
	}

	free(lexer);
	//add cleanup functions if needed
	printf("Programa finalizado.\n");
	return (0);
}
