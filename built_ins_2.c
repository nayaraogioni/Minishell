/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_ins_2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/23 15:49:02 by dopereir          #+#    #+#             */
/*   Updated: 2025/08/23 19:06:53 by dopereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_echo(t_parse_data *pd, t_command *cmd)
{
	int	i;
	int	newline_flag;

	i = 1;
	newline_flag = 0;
	if (cmd->argv[1] == NULL)
	{
		pd->pd_exit_status = 0;
		return (write(STDOUT_FILENO, "\n", 1));
	}
	if (ft_strcmp(cmd->argv[1], "-n") == 0)
	{
		i++;
		newline_flag = 1;
	}
	while (cmd->argv[i])
	{
		write(STDOUT_FILENO, cmd->argv[i], ft_strlen(cmd->argv[i]));
		if (cmd->argv[i] && cmd->argv[i + 1] != NULL)
			write(STDOUT_FILENO, " ", 1);
		i++;
	}
	if (newline_flag == 0)
		write(STDOUT_FILENO, "\n", 1);
	return (pd->pd_exit_status = 0);
}
