/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_refactoring.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 23:53:08 by dopereir          #+#    #+#             */
/*   Updated: 2025/08/07 23:53:14 by dopereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/libft.h"
#include "minishell.h"
#include "parser.h"

//genereally for input setup
int	pre_exec_setups(t_command *cmd, int prev_fd)
{
	if (cmd->heredoc_fd >= 0)
	{
		dup2(cmd->heredoc_fd, STDIN_FILENO);
		close(cmd->heredoc_fd);
		if (prev_fd != -1)
			close(prev_fd);
	}
	else if (cmd->input_file)
	{
		if (set_input(cmd) == -1)
			return (1);
		if (prev_fd != -1)
			close(prev_fd);
	}
	else if (prev_fd != -1)
	{
		dup2(prev_fd, STDIN_FILENO);
		close(prev_fd);
		return (0);
	}
	return (0);
}

//generally for output setup
int	pre_exec_setups_2(t_command *cmd, int c_pipe[2], int has_pipe)
{
	if (cmd->output_file)
	{
		if (set_output(cmd) == -1)
			return (1);
	}
	else if (has_pipe)
	{
		if (dup2(c_pipe[1], STDOUT_FILENO) == -1)
			return (1);
	}
	if (has_pipe)
	{
		close(c_pipe[0]);
		close(c_pipe[1]);
	}
	return (0);
}

int	pos_exec_error_codes(char *cmd_name, int errno_code)
{
	int	exit_code;

	exit_code = 1;
	if (errno_code == ENOENT)
	{
		printf("Command '%s' not found.\n", cmd_name);
		free (cmd_name);
		return (127);
	}
	else if (errno_code == EACCES)
	{
		printf("Permission denied: %s\n", cmd_name);
		free (cmd_name);
		return (126);
	}
	else if (errno_code == EFAULT)
	{
		printf("Error executing '%s': %s\n", cmd_name, strerror(errno_code));
		free (cmd_name);
		return (1);
	}
	free(cmd_name);
	return (exit_code);
}

//Return values:
// 0 -> caller continues the program
// 1 -> caller calls exit(1)
//-1 -> caller breaks
int	pre_exec_prep(t_command *cmd, t_env **env, int n, int cp[2])
{
	bool		is_parent_bt;

	is_parent_bt = is_parent_builtin(cmd->name);
	if (is_parent_bt && n == 1)
	{
		if (cmd->input_file)
			set_input(cmd);
		if (cmd->output_file)
			set_output(cmd);
		run_parent_built(cmd, env);
		return (-1);
	}
	if (cmd->next_is_pipe && pipe(cp) < 0)
		return (1);
	return (0);
}

void	exit_code(t_parse_data *pd, t_env **env, pid_t pids[MAX_ARGS])
{
	int		wstatus;
	int		i;
	char	*exit_str;

	i = 0;
	while (i < pd->n_spawn_pids)
	{
		if (waitpid(pids[i], &wstatus, 0) >= 0)
		{
			if (WIFEXITED(wstatus))
				pd->pd_exit_status = WEXITSTATUS(wstatus);
			else if (WIFSIGNALED(wstatus))
				pd->pd_exit_status = 128 + WTERMSIG(wstatus);
		}
		i++;
	}
	//printf("PD->PD_EXIT_STATUS: %d\n", pd->pd_exit_status);
	exit_str = ft_itoa(pd->pd_exit_status);
	//printf("EXIT_STR = %s\n", exit_str);
	if (!exit_str)
		return ;
	replace_env_value(env, "?", exit_str);
	//ft_setenv(env, "?", exit_str);
	//printf("ENV_TEST = %s\n", ft_getenv(*env, "?"));
	free (exit_str);
}
