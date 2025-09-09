/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handlers.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 10:43:53 by dopereir          #+#    #+#             */
/*   Updated: 2025/09/07 18:56:26 by dopereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <readline/readline.h>

//CTRL-C HANDLER
void	sigint_handler(int signo)
{
	char	nl;

	(void)signo;
	nl = ' ';
	g_heredoc_sig = SIGINT;
	if (RL_ISSTATE(RL_STATE_READCMD))
	{
		printf("MINISHELL>$ %s^C\n", rl_line_buffer);
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
		rl_done = 1;
		ioctl(STDIN_FILENO, TIOCSTI, &nl);
	}
	else
	{
		rl_replace_line("", 0);
		rl_on_new_line();
		write(STDOUT_FILENO, "\n", 1);
	}
}

void	sigquit_handler(int signo)
{
	size_t	total_len;
	size_t	buff_len;
	size_t	i;

	buff_len = 0;
	i = 0;
	if (rl_line_buffer)
		buff_len = ft_strlen(rl_line_buffer);
	total_len = buff_len + 14;
	(void)signo;
	if (RL_ISSTATE(RL_STATE_READCMD))
	{
		write(STDOUT_FILENO, "\r", 1);
		while (i < total_len)
		{
			write(STDOUT_FILENO, " ", 1);
			i++;
		}
		write(STDOUT_FILENO, "\r", 1);
		rl_on_new_line();
		rl_redisplay();
		return ;
	}
	else
		printf("minishell: quit (core dumped)  %s\n", rl_line_buffer);
}

void	heredoc_loop_err_helper(char *line, t_env *env,
	t_command *cmd, int flag)
{
	if (flag == SIGINT)
	{
		replace_env_value(&env, "?", "130");
		free (line);
		return ;
	}
	if (flag == -1)
	{
		printf("minishell: warning: here-document delimited by "
			"end-of-file (wanted '%s')\n", cmd->hd_delim);
		return ;
	}
	if (flag == -2)
	{
		free(line);
		return ;
	}
}

void	signal_err_set(t_env *env, t_lexer *lexer)
{
	replace_env_value(&env, "?", "130");
	g_heredoc_sig = 0;
	lexer->exit_status = 130;
}

//send -1 to get the last exit code (getter)
//send a non(-1) value to set the exit code;
/*int	set_and_get_exit_code(int value)
{
	static int	code = 0;

	if (value == -1)
		return (code);
	code = value;
	return (code);
}*/
