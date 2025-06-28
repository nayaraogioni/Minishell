/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nayara <nayara@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 01:10:10 by dopereir          #+#    #+#             */
/*   Updated: 2025/06/28 13:18:20 by nayara           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
# define MAX_ARGS 1024

# include <signal.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <string.h>
# include <stdbool.h>
# include "libft/libft.h"
# include "lexer.h"
# include "parser.h"


//WHEN PARSING THE DELIMITER SHOULD ALSO BE SPECIFIQ CONTROLS LIKE
// redirections, pipes among other things
//ex:	 mkdir test&&cd test&&touch test.txt&&touch another.txt&&ls > test1.txt #THIS WORKS

typedef struct s_parsephase_data
{
	t_command	*root_command;
}			t_parse_data;



static volatile	int	keepRunning = 1;

// cd /some/path/ && ls | grep *.txt > output.txt && cat output.txt

//main.c
//void	sig_int_handler(int something);
//tokenizer.c
void	print_tokens(t_lexer *shell); //TEMP FUNCTION FOR TESTING
void	clear_token(t_token *tokens, int token_count);
int		token_counter(char *str, char delim);
t_token	*split_tokens(char *str, char delim, t_lexer *lexer);
void	lexing_input(t_lexer *lexer, char delim);
t_command	*parse_function(t_lexer *lexer);
t_command	*parse_sequence(t_lexer *lexer);
t_command	*parse_pipeline(t_lexer *lexer);
t_command	*parse_simple_command(t_lexer *lexer);
int	has_pipes(t_lexer *lexer);
int	has_logical_operators(t_lexer *lexer);
int	count_args(t_lexer *lexer);
void	free_command(t_command *cmd);
int	find_next_pipe(t_lexer *lexer, int start);
int	find_next_logical_operator(t_lexer *lexer, int start);
t_lexer	*create_sublexer(t_lexer *lexer, int start, int end);
void	free_sublexer(t_lexer *sublexer);

//exec_commands.c
//void	execute_command_pwd(t_data *shell);
//void	execute_command_exit(t_data *shell);
#endif
