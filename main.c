/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nayara <nayara@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 23:15:37 by dopereir          #+#    #+#             */
/*   Updated: 2025/06/28 16:10:12 by nayara           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//handle ctrl-D later
// ctrl-c (sigint)
// ctrl-d (EOF signal)

//MAIN PARA TESTE DE PARSING
// Função para imprimir a árvore de comandos (debug)
void print_command_tree(t_command *cmd, int depth)
{
    int i;
    
    if (!cmd)
        return;
    
    // Indentação baseada na profundidade
    for (i = 0; i < depth; i++)
        printf("  ");
    
    // Imprime informações do comando baseado no tipo
    switch (cmd->type)
    {
        case T_WORD:
            printf("SIMPLE_CMD: %s", cmd->name ? cmd->name : "NULL");
            if (cmd->input_file)
                printf(" < %s", cmd->input_file);
            if (cmd->output_file)
                printf(" > %s", cmd->output_file);
            if (cmd->filename && cmd->type == T_REDIR_HEREDOC)
                printf(" << %s", cmd->filename);
            printf("\n");
            
            // Imprime argumentos
            for (i = 0; i < depth + 1; i++)
                printf("  ");
            printf("ARGS: [");
            i = 0;
            while (cmd->argv[i])
            {
                printf("\"%s\"", cmd->argv[i]);
                if (cmd->argv[i + 1])
                    printf(", ");
                i++;
            }
            printf("]\n");
            break;
            
        case T_PIPE:
            printf("PIPELINE (%d commands):\n", cmd->command_count);
            for (i = 0; i < cmd->command_count; i++)
            {
                for (int j = 0; j < depth + 1; j++)
                    printf("  ");
                printf("Command %d:\n", i + 1);
                print_command_tree(cmd->commands[i], depth + 2);
            }
            break;
            
        case T_AND:
            printf("SEQUENCE (%d commands):\n", cmd->command_count);
            for (i = 0; i < cmd->command_count; i++)
            {
                for (int j = 0; j < depth + 1; j++)
                    printf("  ");
                printf("Command %d:\n", i + 1);
                print_command_tree(cmd->commands[i], depth + 2);
            }
            break;
            
        case T_REDIR_HEREDOC:
            printf("HEREDOC_CMD: %s << %s\n", cmd->name ? cmd->name : "NULL", 
                   cmd->filename ? cmd->filename : "NULL");
            break;
            
        case T_REDIR_APPEND:
            printf("APPEND_CMD: %s >> %s\n", cmd->name ? cmd->name : "NULL", 
                   cmd->output_file ? cmd->output_file : "NULL");
            break;
            
        default:
            printf("UNKNOWN_CMD (type: %d)\n", cmd->type);
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
    t_lexer *lexer;
    t_command *cmd;
    char *input;
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
    
    while (keepRunning)
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
        cmd = parse_function(lexer);
        
        if (cmd)
        {
            printf("Parse bem-sucedido!\n");
            printf("\n--- ÁRVORE DE COMANDOS ---\n");
            print_command_tree(cmd, 0);
            free_command(cmd);
        }
        else
        {
            printf("❌ ERRO: Falha no parsing!\n");
        }
        
        // Limpeza
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
    printf("Programa finalizado.\n");
    return (0);
}

// MAIN ALTERADA
/* int main(void)
{
	t_lexer *lexer;
	t_parse_data *parse_data;

	lexer = malloc(sizeof(t_lexer));
	if (!lexer)
		return (1);
	lexer->input = NULL;
	lexer->tokens = NULL;
	lexer->token_count = 0;
	int keepRunning = 1;
	while (keepRunning)
	{
		lexer->input = readline("PROMPT>$ ");
		if (!lexer->input)
		{
			printf("exit\n");
			break;
		}
		if (lexer->input[0] != '\0')
		{
			lexing_input(lexer, ' ');
			if (lexer->token_count > 0)
			{
				printf("\n=== TOKENS ===\n");
				print_tokens(lexer);
				parse_data = parse_input(lexer);
				if (parse_data)
				{
					print_parsed_commnads(parse_data);
					execute_parsed_commands(parse_data);
					free_parse_data(parse_data);
				}
			}
		}
		add_history(lexer->input);
	}
	free(lexer->input);
	lexer->input = NULL;
	if (lexer->tokens)
		clear_token(lexer->tokens, lexer->token_count);
	free(lexer);
	return (0);
} */
//MAIN ORIGINAL
/* int	main(void)
{
	t_lexer	*lexer;

	lexer = malloc(sizeof(t_lexer));
	lexer->input = NULL;
	lexer->tokens = NULL;
	//lexer->path = NULL;
	lexer->token_count = 0;

	int	flag = 1;

	while(flag)
	{
		lexer->input = readline("PROMPT>$ "); // READ (1/2)
		if (lexer->input)
		{
			lexing_input(lexer, ' '); // READ(2/2)
			print_tokens(lexer); // PRINT TOKENS
			// execute(lexer);
			//parse_function(lexer);
				// criar t_parse_data -> 
			
			add_history(lexer->input);
			free (lexer->input);
		}
		//keepRunning = 0;
		printf("KEEP RUNNING: %d\n", keepRunning);
	}
	return (0);
} */
