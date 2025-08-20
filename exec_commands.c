/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_commands.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 22:48:16 by dopereir          #+#    #+#             */
/*   Updated: 2025/07/19 02:13:33 by dopereir         ###   ########.fr       */
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

/*void	exec_parsed_cmds(t_parse_data *pd, t_env **env, t_lexer *lexer)
{
	pid_t		pids[MAX_ARGS];
	t_exec_data	ctx;
	int			rc;

	(void)lexer;
	ctx.fd = -1;
	ctx.i = 0;
	pd->n_spawn_pids = 0;
	while (ctx.i < pd->n_cmds)
	{
		ctx.rc = pre_exec_prep(pd->commands[ctx.i], env, pd->n_cmds, ctx.pipe);
		if (ctx.rc == -1)
			break ;
		if (ctx.rc == 1)
			exit (1);
		pids[ctx.i] = fork();
		if (pids[ctx.i] < 0)
			exit(1);
		if (pids[ctx.i] == 0)
		{
			rc = child_run(pd->commands[ctx.i], ctx.fd, env, ctx.pipe);
			clean_env_list(env);
			exit (rc);
		}
		parent_run(pd->commands[ctx.i], &ctx.fd, ctx.pipe);
		ctx.i++;
		pd->n_spawn_pids++;
	}
	if (ctx.fd != -1)
		close(ctx.fd);
	exit_code(pd, env, pids);
}*/

static void	handle_child_process(t_command *cmd, int fd, t_env **env, \
	int pipe[2])
{
	int	rc;

	rc = child_run(cmd, fd, env, pipe);
	clean_env_list(env);
	_exit(rc);
}

static void	handle_parent_process(t_command *cmd, int *fd, int pipe[2])
{
	parent_run(cmd, fd, pipe);
}

static int	spawn_processes(t_parse_data *pd, t_env **env, pid_t *pids)
{
	t_exec_data	ctx;

	ctx.fd = -1;
	ctx.i = 0;
	ctx.pipe[0] = -1;
	ctx.pipe[1] = -1;
	pd->n_spawn_pids = 0;
	while (ctx.i < pd->n_cmds)
	{
		ctx.rc = pre_exec_prep(pd->commands[ctx.i], env, pd->n_cmds, ctx.pipe);
		if (ctx.rc == -1)
		{
			parent_run(pd->commands[ctx.i], &ctx.fd, ctx.pipe);
			break ;
		}
		if (ctx.rc == 1)
			exit(1);
		pids[ctx.i] = fork();
		if (pids[ctx.i] < 0)
			exit(1);
		if (pids[ctx.i] == 0)
			handle_child_process(pd->commands[ctx.i], ctx.fd, env, ctx.pipe);
		handle_parent_process(pd->commands[ctx.i], &ctx.fd, ctx.pipe);
		ctx.i++;
		pd->n_spawn_pids++;
	}
	if (ctx.fd != -1)
		close(ctx.fd);
	return (0);
}

void	exec_parsed_cmds(t_parse_data *pd, t_env **env, t_lexer *lexer)
{
	pid_t	pids[MAX_ARGS];
	int		rc;

	(void)lexer;
	rc = spawn_processes(pd, env, pids);
	if (rc == -1)
		printf("SPAWN_PROCESS %d\n", rc);
	if (rc == 0)
		exit_code(pd, env, pids);
}
