/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 01:10:10 by dopereir          #+#    #+#             */
/*   Updated: 2025/07/03 23:55:16 by dopereir         ###   ########.fr       */
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
# include <time.h>
# include <stdbool.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <fcntl.h>
# include "libft/libft.h"
# include "lexer.h"
# include "parser.h"


//WHEN PARSING THE DELIMITER SHOULD ALSO BE SPECIFIQ CONTROLS LIKE
// redirections, pipes among other things
//ex:	 mkdir test&&cd test&&touch test.txt&&touch another.txt&&ls > test1.txt #THIS WORKS

typedef struct s_parsephase_data
{
	t_command		*commands[MAX_ARGS];
	int				n_cmds;
}			t_parse_data;

static volatile sig_atomic_t	g_heredoc_sig = 0;
extern	char		**environ;

// cd /some/path/ && ls | grep *.txt > output.txt && cat output.txt

//main.c
//void	sig_int_handler(int something);
//tokenizer.c
void	print_tokens(t_lexer *shell); //TEMP FUNCTION FOR TESTING
void	clear_token(t_token *tokens, int token_count);
int		token_counter(char *str, char delim);
t_token	*split_tokens(char *str, char delim, t_lexer *lexer);
void	lexing_input(t_lexer *lexer, char delim);
//parser.c
t_command	*init_command(void);
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
//collect_commands.c
void			free_parsed_data(t_parse_data *parsed_data);
t_parse_data	format_parsed_data(t_lexer *lexer);
void			print_parsed_data(const t_parse_data *pd);
//execute_helpers.c
char			*cmd_path_generator(char	*cmd_name);
//exec_commands.c
void			exec_parsed_cmds(t_parse_data *pd);
//redirections_utils.c
int				set_output(t_command *cmd);
int				set_input(t_command *cmd);
int				set_pipe(int *read_fd, int *write_fd);
int				set_heredoc(char *delim);
#endif
