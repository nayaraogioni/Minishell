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

//RETURNS 0 ON SUCESS -1 ON FAILURE
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

static void	heredoc_handler(int	ignore)
{
	ignore = 1;
	g_heredoc_sig = 1;
	write(ignore, "\n", 1);
}

//SET A SIGINT HANDLER SO IF ^C during the heredoc cancels the here-doc
int	set_heredoc(char *delim)
{
	int					pipefd[2];
	//char				*line;
	struct sigaction	orig_int;
	struct sigaction	ignore_quit;
	struct sigaction	sa;

	if (pipe(pipefd) < 0)
	{
		perror("Pipe (heredoc)");
		return (-1);
	}
	sigemptyset(&sa.sa_mask);
	sa.sa_flags   = SA_RESTART;
	sa.sa_handler = heredoc_handler;
	sigaction(SIGINT, &sa, &orig_int);
	ignore_quit.sa_handler = SIG_IGN;
	sigaction(SIGQUIT, &ignore_quit, NULL);
	while (!g_heredoc_sig) {
        char *line = readline("> ");
        if (!line) break;
        if (strcmp(line, delim) == 0) {
            free(line);
            break;
        }
        write(pipefd[1], line, strlen(line));
        write(pipefd[1], "\n", 1);
        free(line);
    }

    // restore original handler
    sigaction(SIGINT,  &orig_int, NULL);
    sigaction(SIGQUIT, &ignore_quit, NULL);

    close(pipefd[1]);
    if (g_heredoc_sig) {
        g_heredoc_sig = 0;   // reset for next time
        close(pipefd[0]);
        return -1;           // signal “abort heredoc”
    }
    return pipefd[0];
}
