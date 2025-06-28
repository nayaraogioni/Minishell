/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_commands.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 22:48:16 by dopereir          #+#    #+#             */
/*   Updated: 2025/06/25 22:46:14 by dopereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//#include "lexer.h"
#include "lexer.h"
#include "minishell.h"
#include "parser.h"
#include <csignal>
#include <unistd.h>
//#include "parser.h"

int	argv_delimiter(char *arg)
{
	char	*target_tokens[6];
	int		i;

	target_tokens[0] = "&&";
	target_tokens[1] = ">";
	target_tokens[2] = "<<";
	target_tokens[3] = ">>";
	target_tokens[4] = "|";
	target_tokens[5] = NULL;
	i = 0;
	while (target_tokens[i] != NULL)
	{
		if (ft_strcmp(arg, target_tokens[i]) == 0)
			return (1);
		i++;
	}
	return (0);
}

void	executeold(t_lexer *lexer)
{
	char	full_path[1024];
	int		i;
	int		j;
	int		found_path_flag = 0;
	pid_t	pid;

	//adapt to use curr_cmd, we shall receive the t_parse_data structure and execute one by one

	i = 0;
	j = 0;
	while (i < lexer->token_count && argv_delimiter(lexer->tokens[i].text) == 0) //will create the arguments of the current command
	{
		lexer->args[j] = ft_strdup(lexer->tokens[i].text);
		i++;
		j++;
	}
	lexer->args[j] = NULL;
	found_path_flag = cmd_path_generator(lexer , full_path);
	if (found_path_flag)
	{
		pid = fork();
		if (pid == 0) //child process to execute
		{
			execve(full_path, lexer->args, NULL);
		}
		else if (pid > 0) //wait for the child process to finish
			wait(NULL);
		else
		{
			perror("fork in exec_commands.c failed");
		}
	}
	for (i = 0; lexer->args[i] != NULL; i++)
		free(lexer->args[i]);
}

void	exec_parsed_cmds(t_parse_data *pd)
{
	int		prev_fd;
	pid_t	pids[MAX_ARGS];
	int		pipe_read;
	int		pipe_write;
	int		i;
	int		heredoc_fd;
	t_command	*cmd;

	for (i = 0; i < pd->n_cmds; i++)
	{
		cmd = pd->commands[i];
		pipe_read = -1;
		pipe_write = -1;
		if (cmd->type == T_PIPE)
		{
			if (set_pipe(&pipe_read, &pipe_write) < 0)
			{
				printf("Error on set_pipe\n");
				exit(1);
			}
			pids[i] = fork();//FORK THE CHILD
			if (pids[i] < 0)
			{
				perror("fork");
				exit(1);
			}
			if (pids[i] == 0) //CHILD RUNTIME
			{
				if (cmd->type == T_REDIR_HEREDOC) //IS HEREDOC?
				{
					heredoc_fd = set_heredoc();//DELIMITER)
					if (heredoc_fd < 0)
						exit(1);
					dup2(heredoc_fd, STDIN_FILENO);
					close(heredoc_fd);
				}
				else if (cmd->input_file) //SETUP STDIN
					set_input(cmd);
				else if (prev_fd != -1)
					dup2(prev_fd, STDIN_FILENO);
				if (cmd->output_file) //SETUO STDOUT
					set_output(cmd);
				else if (pipe_write != -1)
					dup2(pipe_write, STDOUT_FILENO);
				if (prev_fd   != -1) // CLOSE FDS WE DONT NEED
					close(prev_fd);
				if (pipe_read  != -1)
					close(pipe_read);
				if (pipe_write != -1)
					close(pipe_write);
				execve(cmd->path, cmd->argv, environ);
				perror("execve failed");// only if exec fails
				exit(127);
			}
			//PARENT RUNTIME
			if (prev_fd   != -1)
				close(prev_fd);
			if (pipe_write != -1)
				close(pipe_write);
			prev_fd = pipe_read; //NEXT INTERATION CHILDS READ FROM HERE
		}
		for (i = 0; i < pd->n_cmds; i++)
			waitpid(pids[i], NULL, 0);
	}
}
