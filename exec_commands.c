/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_commands.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 22:48:16 by dopereir          #+#    #+#             */
/*   Updated: 2025/03/21 23:48:13 by dopereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	execute_command_pwd(t_data *shell)//later change to implement proper command structure
{
	char	*buf;

	buf = NULL;
	if (ft_strncmp(shell->tokens[0], "pwd", 3) == 0)
	{
		buf = getcwd(NULL, 0);
		printf("%s\n", buf);
		free(buf);
		buf = NULL;
	}
}

void	execute_command_exit(t_data *shell)
{
	if (ft_strncmp(shell->tokens[0], "exit", 4) == 0)
	{
		printf("INTERRUPUT BY exit COMMAND\n");
		keepRunning = 0;
		exit(0);
	}
}
