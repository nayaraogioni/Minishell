/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 21:59:46 by dopereir          #+#    #+#             */
/*   Updated: 2025/07/04 23:00:53 by dopereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/libft.h"
#include "minishell.h"
#include "parser.h"
#include <fcntl.h>
#include <readline/readline.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// 1. call this function inside child process BEFORE exec but AFTER forking
// 2. this funcition will change the FD opened with open(filename) to the
// stdout_fileno
int	set_output(t_command *cmd)
{
	int	fd_target;
	int	flags;

	if (!cmd || !cmd->output_file)
		return (-1);
	flags = O_WRONLY | O_CREAT | O_CLOEXEC;
	if (cmd->type == T_REDIR_OUT)
		flags |= O_TRUNC;
	else if (cmd->type == T_REDIR_APPEND)
		flags |= O_APPEND;
	else
		return (-1);
	fd_target = open(cmd->output_file, flags, 0644);
	if (fd_target < 0)
		return (perror("minishell: (stdout redirection error)"), -1);
	if (dup2(fd_target, STDOUT_FILENO) < 0)
	{
		perror("minishell: (stdout redirection error)");
		close(fd_target);
		return (-1);
	}
	close(fd_target);
	return (0);
}

//RETURNS 0 ON SUCESS -1 ON FAILURE
int	set_input(t_command *cmd)
{
	int	fd_source;

	if (!cmd || !cmd->input_file)
		return (-1);
	fd_source = open(cmd->input_file, O_RDONLY);
	if (fd_source < 0)
	{
		perror("minishell: (input redirection)");
		return (-1);
	}
	if (dup2(fd_source, STDIN_FILENO) < 0)
	{
		perror("minishell: dup2: (input redirection)");
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
