/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/08 11:56:17 by dopereir          #+#    #+#             */
/*   Updated: 2025/06/09 22:14:42 by dopereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//#include "libft/libft.h"
#include "minishell.h"
#include <time.h>
#include <unistd.h>

//parse through tokens and create meaning modules of data (parser responsability)

//use dup2 to redirect the fd 1(stdout) to target fd.

/*
for example:

cat < input.txt | grep foo > output.txt

### command[0]: cat < input.txt
{
	type		= T_PIPE,		// because it's followed by a |
	name		= "cat",
	path		= "/bin/cat",	// resolved via $PATH
	argv		= ["cat", NULL],
	input_file	= "input.txt",
	output_file	= NULL
}

### command[1]: grep foo > output.txt
{
	type		= T_WORD,
	name		= "grep",
	path		= "/bin/grep",
	argv		= ["grep", "foo", NULL],
	input_file	= NULL,
	output_file	= "output.txt"
}

So the command array has 2 elements, each with redirection data already populated.

*/

//this function can be called at execute func
/*void	redirections(t_command **commands, int n_cmds)
{
	int	i;
	int	fd_target;
	int	saved_stdout;
	pid_t	pid;

	i = 0;
	while (i < n_cmds)
	{
		if (commands[i]->type == T_REDIR_OUT) //pay attention, we must return error when the file doest have permission
		{
			saved_stdout = dup(STDOUT_FILENO);
			fd_target = open(commands[i+1]->filename, O_WRONLY | O_CREAT, 0664);
			if (fd_target < 0)
			{
				perror("Error at open() function call");
				exit(EXIT_FAILURE);
			}
			dup2(fd_target, STDOUT_FILENO);
			close(fd_target);
			//exec and fork
			pid = fork();
			if (pid == 0)
				execve(commands[i-1]->path, commands[i-1]->argv, NULL);
			else
				wait(NULL);
			close(fd_target);
			dup2(saved_stdout, STDOUT_FILENO);
			break ;
		}
		i++;
	}
}*/

int	main(void)
{
	int	pipefd[2];
	t_command	*commands[3];

	commands[0] = malloc(sizeof(t_command));
	commands[1] = malloc(sizeof(t_command));
	commands[2] = NULL;

	commands[0]->type = T_PIPE;
	commands[0]->name = "cat";
	commands[0]->path = "/usr/bin/cat";
	commands[0]->argv[0] = "cat";
	commands[0]->argv[1] = NULL;
	commands[0]->input_file = "input.txt";
	commands[0]->output_file = NULL;

	commands[1]->type = T_REDIR_OUT;
	commands[1]->name = "grep";
	commands[1]->path = "/usr/bin/grep";
	commands[1]->argv[0] = "grep";
	commands[1]->argv[1] = "foo";
	commands[1]->argv[2] = NULL;
	commands[1]->input_file = NULL;
	commands[1]->output_file = "output.txt";

	pipe(pipefd);// pipefd[0] = read end, pipefd[1] = write end

	for (int i = 0; commands[i] != NULL; i++)
	{
		pid_t	pid = fork();
		if (pid == 0)
		{
			if (i == 0)
			{
				dup2(pipefd[1], STDOUT_FILENO);//send output from command 1 to pipe
				close(pipefd[0]);
				close(pipefd[1]);
			}
			else if (i == 1)
			{
				dup2(pipefd[0], STDIN_FILENO);//receive input from command 0 to pipe
				close(pipefd[0]);
				close(pipefd[1]);
			}

			set_input(commands[i]);
			set_output(commands[i]);
			execve(commands[i]->path, commands[i]->argv, environ);
			perror("execve");
			exit(EXIT_FAILURE);
		}
		else
			wait(NULL);
		close(pipefd[0]);
		close(pipefd[1]);
		//for (int j = 0; j < 2; j++)
			//wait(NULL);
	}
	free(commands[0]);
	free(commands[1]);
}
