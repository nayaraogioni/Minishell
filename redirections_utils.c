/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 21:59:46 by dopereir          #+#    #+#             */
/*   Updated: 2025/06/26 21:59:49 by dopereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"
#include <csignal>
#include <readline/readline.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

//call this function inside child process BEFORE exec
// but after forking
// this funcition basically will change the fd opened by the filename in the cmd
// to the stdout_fileno
int	set_output(t_command *cmd)
{
	int	fd_target;
	int	flags;

	if (!cmd || !cmd->output_file)
		return (-1);
	flags = O_WRONLY | O_CREAT;
	if (cmd->type == T_REDIR_OUT)
		flags |= O_TRUNC;
	else if (cmd->type == T_REDIR_APPEND)
		flags |= O_APPEND;
	else
		return (-1);
	fd_target = open(cmd->output_file, flags, 0644);
	if (fd_target < 0)
		return (perror("Open (stdout redirection error)"), -1);
	if (dup2(fd_target, STDOUT_FILENO) < 0)
	{
		perror("dup2 (stdout redirection)");
		close(fd_target);
		return (-1);
	}
	close(fd_target);
	return (0);
}

int	set_input(t_command *cmd)
{
	int	fd_source;

	if (!cmd || !cmd->input_file)
		return (-1);
	fd_source = open(cmd->input_file, O_RDONLY);
	if (fd_source < 0)
	{
		perror("open (input redirection)");
		return (-1);
	}
	if (dup2(fd_source, STDIN_FILENO) < 0)
	{
		perror("dup2 (input redirection)");
		close(fd_source);
		return (-1);
	}
	close(fd_source);
	return (0);
}

int	set_pipe(int *read_fd, int *write_fd)
{
	int	fds[2];

	*read_fd = -1;
	*write_fd = -1;
	if (pipe(fds) < 0)
	{
		perror("pipe");
		return (-1);
	}
	*read_fd = fds[0];
	*write_fd = fds[1];
	return (0);
}

//CHANGE STANDARD FUNCTION TO FT_FUNCTIONS
int	set_heredoc(const char *delim)
{
	int		pipefd[2];
	char	*line;

	if (pipe(pipefd) < 0)
	{
		perror("Pipe (heredoc)");
		return (-1);
	}
	while (1)
	{
		line = readline("> ");
		if (!line)
			break;
		if (strcmp(line, delim) == 0)
		{
			free(line);
			break;
		}
		write(pipefd[1], line, strlen(line));
		write(pipefd[1], "\n", 1);
		free(line);
	}
	close(pipefd[1]);
	return (pipefd[0]);
}
