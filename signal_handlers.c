/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handlers.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 10:43:53 by dopereir          #+#    #+#             */
/*   Updated: 2025/07/22 10:43:57 by dopereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <readline/readline.h>

//CTRL-C HANDLER
void	sigint_handler(int signo)
{
	(void)signo;
	if (g_heredoc_sig != 1)
	{
		printf("MINISHELL>$ %s^C\n", rl_line_buffer);
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
	}
	else if (g_heredoc_sig == 1)
	{
		rl_replace_line("", 0);
		printf("> ^C\n");
		rl_on_new_line();
		rl_redisplay();
		g_heredoc_sig = 0;
	}
}

//CTRL-\ HANDLER (must ignore)
