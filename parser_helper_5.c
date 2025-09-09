/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_helper_5.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nayara <nayara@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 17:39:40 by nayara            #+#    #+#             */
/*   Updated: 2025/09/09 17:42:59 by nayara           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	finalize_command(t_command *cmd, t_pbuilder *pb)
{
	if (acc_module(cmd, pb) == -1)
		return (-1);
	cmd->argv[pb->arg_index] = NULL;
	if (!cmd->name && pb->arg_index == 0
		&& !cmd->input_file && !cmd->output_file && !cmd->hd_delim)
	{
		free_command(cmd);
		return (-1);
	}
	return (0);
}

int	set_and_get(int value)
{
	static int	flag;

	flag = 1;
	if (value == -1)
		return (flag);
	else
		flag = value;
	return (flag);
}
