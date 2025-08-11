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
#include <readline/readline.h>
#include <signal.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

void	heredoc_sig_handler(int ignore)
{
	(void)ignore;
	g_heredoc_sig = 1;
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
	rl_done = 1;
}

static void	heredoc_loop_helper(t_command *cmd, int pipefd[2], t_env *env)
{
	char	*line;
	char	*expanded_line;

	while (!g_heredoc_sig)
	{
		line = readline("> ");
		if (!line)
		{
			printf("minishell: warning: here-document delimited by "\
				"end-of-file (wanted '%s')\n", cmd->hd_delim);
			break ;
		}
		if (ft_strcmp(line, cmd->hd_delim) == 0)
		{
			free(line);
			break ;
		}
		//build a function to expand the variable
		expanded_line = expand_heredoc_line(line, env);
		free(line);
		write(pipefd[1], expanded_line, ft_strlen(expanded_line));
		write(pipefd[1], "\n", 1);
		free(expanded_line);
	}
}

static void	heredoc_setup_signal_helper(struct sigaction *orig_int)
{
	struct sigaction	sa;

	sa.sa_handler = heredoc_sig_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, orig_int);
}

static int	process_heredoc(t_command *cmd, t_env *env)
{
	int	pipefd[2];

	if (pipe(pipefd) < 0)
		return (-1);
	cmd->heredoc_fd = pipefd[0];
	g_heredoc_sig = 0;
	heredoc_loop_helper(cmd, pipefd, env);
	close(pipefd[1]);
	if (g_heredoc_sig)
	{
		close(cmd->heredoc_fd);
		return (-1);
	}
	return (0);
}

int	handle_all_heredocs(t_parse_data *pd, t_env *env)
{
	struct sigaction	orig_int;
	t_command			*cmd;
	int					i;

	i = 0;
	heredoc_setup_signal_helper(&orig_int);
	while (i < pd->n_cmds)
	{
		cmd = pd->commands[i];
		if (!cmd->hd_delim)
		{
			i++;
			continue ;
		}
		if (process_heredoc(cmd, env) < 0)
		{
			sigaction(SIGINT, &orig_int, NULL);
			return (-1);
		}
		i++;
	}
	sigaction(SIGINT, &orig_int, NULL);
	return (0);
}
