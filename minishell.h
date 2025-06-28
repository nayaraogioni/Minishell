/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 01:10:10 by dopereir          #+#    #+#             */
/*   Updated: 2025/06/09 22:14:31 by dopereir         ###   ########.fr       */
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
# include <sys/types.h>
# include <sys/wait.h>
# include <fcntl.h>
//# include "libft/libft.h"
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

static volatile	int	keepRunning = 1;
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
//exec_commands.c
int		cmd_path_generator(t_lexer *lexer, char *full_path);
void	execute(t_lexer *lexer);
//redirections_utils.c
int		set_output(t_command *cmd);
int		set_input(t_command *cmd);
int		set_pipe(int *read_fd, int *write_fd);
int		set_heredoc(const char *delim);
#endif
