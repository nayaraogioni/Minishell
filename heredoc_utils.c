/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nayara <nayara@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/08 11:56:17 by dopereir          #+#    #+#             */
/*   Updated: 2025/09/09 17:20:16 by nayara           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/libft.h"
#include "minishell.h"
#include "parser.h"
#include <readline/readline.h>

static void	hd_expand_helper(t_command *cmd, char *line, char *expanded_line,
					int pipefd[2])
{
	write(pipefd[1], expanded_line, ft_strlen(expanded_line));
	write(pipefd[1], "\n", 1);
	if (cmd->hd_expand)
		free(expanded_line);
	else
		free(line);
}

static void	heredoc_loop_helper(t_command *cmd, int pipefd[2], t_env *env)
{
	char	*line;
	char	*expanded_line;

	while (1)
	{
		line = readline("> ");
		if (g_heredoc_sig == SIGINT)
			return (heredoc_loop_err_helper(line, env, cmd, SIGINT));
		if (!line)
			return (heredoc_loop_err_helper(line, env, cmd, -1));
		if (ft_strcmp(line, cmd->hd_delim) == 0)
			return (heredoc_loop_err_helper(line, env, cmd, -2));
		cmd->hd_expand = set_and_get(-1);
		if (cmd->hd_expand == 1)
		{
			expanded_line = expand_heredoc_line(line, env);
			free(line);
		}
		else
			expanded_line = line;
		hd_expand_helper(cmd, line, expanded_line, pipefd);
	}
}

static void	heredoc_setup_signal_helper(struct sigaction *sig)
{
	struct sigaction	sa;

	sa.sa_handler = heredoc_sig_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, sig);
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
	if (g_heredoc_sig == SIGINT)
	{
		close(cmd->heredoc_fd);
		return (-1);
	}
	return (0);
}

int	handle_all_heredocs(t_parse_data *pd, t_env *env)
{
	t_command			*cmd;
	int					i;

	i = 0;
	heredoc_setup_signal_helper(&pd->sig_catcher);
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
			sigaction(SIGINT, &pd->sig_catcher, NULL);
			return (-1);
		}
		i++;
	}
	sigaction(SIGINT, &pd->sig_catcher, NULL);
	return (0);
}
