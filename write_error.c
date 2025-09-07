/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   write_error.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 19:30:46 by dopereir          #+#    #+#             */
/*   Updated: 2025/09/07 19:55:54 by dopereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	write_error_case(char *cmd_name, int errno_code)
{
	if (errno_code == ENOENT)
	{
		write(STDERR_FILENO, "Command '", 9);
		write(STDERR_FILENO, cmd_name, ft_strlen(cmd_name));
		write(STDERR_FILENO, "' not found.\n", 14);
		return ;
	}
	else if (errno_code == EACCES)
	{
		write(STDERR_FILENO, "Permission denied: ", 19);
		write(STDERR_FILENO, cmd_name, ft_strlen(cmd_name));
		write(STDERR_FILENO, "\n", 1);
		return ;
	}
}

/*	else if (errno_code == EFAULT)
	{
		write(STDERR_FILENO, "Error executing '", 17);
		write(STDERR_FILENO, cmd_name, ft_strlen(cmd_name));
		write(STDERR_FILENO, "': ", 3);
		err_msg = strerror(errno_code);
		write(STDERR_FILENO, err_msg, ft_strlen(err_msg));
		write(STDERR_FILENO, "\n", 1);
		return ;
	}*/
