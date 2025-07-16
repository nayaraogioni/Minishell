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

#include "minishell.h"

static int	create_pipe(int pipefd[2])
{
	if (pipe(pipefd) < 0)
	{
		perror("Pipe (heredoc)");
		return (-1);
	}
	return (0);
}

static void	restore_signals(const struct sigaction *orig)
{
	sigaction(SIGINT, orig, NULL);
	sigaction(SIGQUIT, &(struct sigaction){.sa_handler = SIG_DFL}, NULL);
}

static void	setup_signals(struct sigaction *orig)
{
	struct sigaction	sa;
	struct sigaction	ign;

	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	sa.sa_handler = heredoc_sig_handler;
	sigaction(SIGINT, &sa, orig);
	ign.sa_handler = SIG_IGN;
	sigemptyset(&ign.sa_mask);
	ign.sa_flags = 0;
	sigaction(SIGQUIT, &ign, NULL);
}

static int	read_heredoc(int write_fd, char *delim, int *got_sig)
{
	char	*line;

	while (!*got_sig)
	{
		line = readline("> ");
		if (!line)
		{
			*got_sig = 1;
			break ;
		}
		if (ft_strcmp(line, delim) == 0)
		{
			free(line);
			break ;
		}
		write(write_fd, line, strlen(line));
		write(write_fd, "\n", 1);
		free(line);
	}
	return (*got_sig);
}

int	set_heredoc(char *delim)
{
	int					pipefd[2];
	struct sigaction	orig_int;
	int					aborted;

	if (create_pipe(pipefd) < 0)
		return (-1);
	setup_signals(&orig_int);
	aborted = read_heredoc(pipefd[1], delim, (int *)&g_heredoc_sig);
	restore_signals(&orig_int);
	close(pipefd[1]);
	if (aborted)
	{
		ft_printf("minishell: warning: here-document "
			"delimited by end-of-file (wanted '%s')\n", delim);
		g_heredoc_sig = 0;
		return (pipefd[0]);
	}
	return (pipefd[0]);
}
