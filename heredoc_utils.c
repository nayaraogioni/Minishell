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

#include "libft/libft.h"
#include "minishell.h"
#include "parser.h"
#include <asm-generic/ioctls.h>
#include <signal.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

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
	struct sigaction	dfl;

	ft_memset(&dfl, 0, sizeof(dfl));

	sigaction(SIGINT, orig, NULL);//restore sigint
	dfl.sa_handler = SIG_DFL;//restore sigquit to default
	sigemptyset(&dfl.sa_mask);
	dfl.sa_flags = 0;
	sigaction(SIGQUIT, &dfl, NULL);
}


static void	setup_signals(struct sigaction *orig)
{
	struct sigaction	sa;
	struct sigaction	ign;

	ft_memset(&sa, 0, sizeof(sa));
	ft_memset(&ign, 0, sizeof(ign));

	sa.sa_handler = heredoc_sig_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, orig);

	ign.sa_handler = SIG_IGN;
	sigemptyset(&ign.sa_mask);
	ign.sa_flags = 0;
	sigaction(SIGQUIT, &ign, NULL);
}

static int	read_heredoc(int write_fd, char *delim)
{
	char	*line;

	while (!g_heredoc_sig)
	{
		line = readline("> ");
		if (!line)
		{
			g_heredoc_sig = 1;
			break ;
		}
		if (ft_strcmp(line, delim) == 0)
		{
			free(line);
			break ;
		}
		write(write_fd, line, ft_strlen(line));
		write(write_fd, "\n", 1);
		free(line);
	}
	return (g_heredoc_sig);
}

int	set_heredoc(char *delim)
{
	int					pipefd[2];
	struct sigaction	orig_int;
	struct sigaction	parent_int;
	struct sigaction	ign;
	int					aborted;
	pid_t				pid;
	int					status;

	if (create_pipe(pipefd) < 0)
		return (-1);
	pid = fork();
	if (pid < 0)
	{
		perror("minishell: heredoc fork:");
		close(pipefd[0]);
		close(pipefd[1]);
		return (-1);
	}
	else if (pid == 0) //processo filho
	{
		close(pipefd[0]);

		setup_signals(&orig_int);
		aborted = read_heredoc(pipefd[1], delim);
		restore_signals(&orig_int);
		close(pipefd[1]);
		if (aborted == 1)
			_exit(1);
		else if (aborted == 0)
			_exit(0);
	}
	close(pipefd[1]);

	sigaction(SIGINT, NULL, &parent_int);
	ign.sa_handler = SIG_IGN;
	sigemptyset(&ign.sa_mask);
	ign.sa_flags = 0;
	sigaction(SIGINT, &ign, NULL);//ignore SIGINT

	waitpid(pid, &status, 0);

	sigaction(SIGINT, &parent_int, NULL);
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
	{
		g_heredoc_sig = 0;
		return (pipefd[0]);
	}
	if (WIFEXITED(status) && WEXITSTATUS(status) == 1)
	{
		ft_printf("minishell: warning: here-document "
			"delimited by end-of-file (wanted '%s')\n", delim);
		//return (pipefd[0]);
	}
	return (pipefd[0]);
}

//TRY NEW APPROACH OF NOT CREATING A PROCESS TO EXECUTE HEREDOC

void	heredoc_sig_handler(int ignore)
{
	(void)ignore;
	g_heredoc_sig = 1;
	//printf("GOT HEREDOC SIG HANDLER\n");
	rl_on_new_line();
	rl_replace_line("", 0);
	//printf("> ^C\n");
	//write(STDOUT_FILENO, "\n", 1);
	//rl_on_new_line();
	rl_redisplay();
	rl_done = 1;
}

int	handle_all_heredocs(t_parse_data *pd)
{
	struct sigaction	orig_int;
	struct sigaction	sa;
	t_command			*cmd;
	int					i;

	i = 0;
	sa.sa_handler = heredoc_sig_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, &orig_int);
	while (i < pd->n_cmds)
	{
		cmd = pd->commands[i];
		if (!cmd->hd_delim)//skips commands with no heredocs
		{
			i++;
			continue ;
		}

		int pipefd[2];
		if (pipe(pipefd) < 0)
			return (-1);
		int	rd = pipefd[0];
		//int	wr = pipefd[1];
		cmd->heredoc_fd = rd;
		//close(pipefd[0]);

		g_heredoc_sig = 0;
		while (!g_heredoc_sig)
		{
			char *line = readline("> ");
			if (!line)  // Ctrl-D
			{
				ft_printf("minishell: warning: here-document delimited by end-of-file (wanted '%s')\n", cmd->hd_delim);
				//write(STDOUT_FILENO, "\n", 1);
				//g_heredoc_sig = 1;
				break;
			}
			if (ft_strcmp(line, cmd->hd_delim) == 0)
			{
				free(line);
				break;
			}
			write(pipefd[1], line, ft_strlen(line));
			write(pipefd[1], "\n", 1);
			free(line);
		}
		close(pipefd[1]);
		if (g_heredoc_sig)
		{
			// Ctrl-C: clean up and abort
			close(cmd->heredoc_fd);
			sigaction(SIGINT, &orig_int, NULL);
			return (-1);
		}
		g_heredoc_sig = 0;  // reset for next heredoc
		i++;
	}
	sigaction(SIGINT, &orig_int, NULL);
	return 0;
}
