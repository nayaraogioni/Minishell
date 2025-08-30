/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_commands.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 22:48:16 by dopereir          #+#    #+#             */
/*   Updated: 2025/08/27 21:41:13 by dopereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/libft.h"
#include "minishell.h"
#include "parser.h"
#include <errno.h>
#include <signal.h>
#include <time.h>
#include <unistd.h>

//return a code that the caller must attribute to a exit() call
// 0 for sucess
// 1 for general error
// another codes depending of the error type
int	child_run(t_command *cmd, int fd, t_env **env, int c_pipe[2])
{
	char	**child_env;
	char	*tmp_cmd_name;
	int		return_code;

	if (pre_exec_setups(cmd, fd) == 1)
		return (1);
	if (pre_exec_setups_2(cmd, c_pipe, cmd->next_is_pipe) == 1)
		return (1);
	cmd->path = cmd_path_generator(cmd->name, *env);
	tmp_cmd_name = ft_strdup(cmd->name);
	child_env = env_to_array(*env);
	execve(cmd->path, cmd->argv, child_env);
	free (cmd->path);
	free_env_array(child_env, list_lenght(*env));
	child_env = NULL;
	return_code = (pos_exec_error_codes(tmp_cmd_name, errno));
	tmp_cmd_name = NULL;
	return (return_code);
}

void	parent_run(t_command *cmd, int *fd, int pipe_var[2])
{
	if (cmd->next_is_pipe)
		close(pipe_var[1]);
	if (*fd != -1)
		close(*fd);
	if (cmd->next_is_pipe)
		*fd = pipe_var[0];
	else
		*fd = -1;
}

void	handle_child_process(t_command *cmd, int fd, t_env **env,
			int pipe[2])
{
	int	rc;

	rc = child_run(cmd, fd, env, pipe);
	clean_env_list(env);
	_exit(rc);
}

void	handle_parent_process(t_command *cmd, int *fd, int pipe[2])
{
	parent_run(cmd, fd, pipe);
}

void	exec_parsed_cmds(t_parse_data *pd, t_env **env, t_lexer *lexer)
{
	pid_t	pids[MAX_ARGS];
	int		rc;

	(void)lexer;
	rc = spawn_processes(pd, env, pids);
	if (rc == 0)
		exit_code(pd, env, pids);
}
