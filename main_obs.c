/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 00:40:30 by dopereir          #+#    #+#             */
/*   Updated: 2025/05/27 18:30:10 by dopereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <string.h>
#include "minishell.h"
//use -lreadline at compilation

//static volatile	int	keepRunning = 1;

void	sig_int_handler(int something)
{
	something = 0;
	printf("%d INTERRUPUT BY SIGNAL", something);
	keepRunning = 0;
	exit(0);
}

/*
OPTIMIZATIONS
START TO HANDLE AND PARSE THE TOKENS

FILE DESCRIPTORS TO HANDLE REDIRECTIONS getpid() to get process pid

TO USE EXECVE WE SHOULD FORK(clone) a processe pid_t pid = fork();
use waitpid to run the child first, prevente the child process to become a zombie
the target of waitpid must be the parent process
use exit to end child process

GETCWD RETURNS CURRENT DIR PATH
CHDIR CHANGES CURRENTE DIR OF THE CALLING PROCESS TO THE DIR SPECIFIED IN THE ARGUMENTE
*/
int	main(void)
{
	t_data	*shell;
	//char	*pwd;

	signal(SIGINT, sig_int_handler);

	shell = malloc(sizeof(t_data));
	init_data(shell);

	while(keepRunning) //REPL
	{
		shell->input = readline("PROMPT>$ "); //READ (R)
		if (shell->input)
		{
			tokenize_input(shell, ' ');// EVAL
			execute_command_pwd(shell); // PRINT
			execute_command_exit(shell); // PRINT
			//printf("Here is your input: <%s>\n", shell->input);
			add_history(shell->input);
			//EXEC COMMANDS
			free (shell->input);
		}
		//pwd = getcwd(NULL, 0);
		//printf("pwd: %s\n", pwd);
		print_tokens(shell);
	}
	return (0);
}