/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_commands_helper.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 19:57:46 by dopereir          #+#    #+#             */
/*   Updated: 2025/08/23 14:56:08 by dopereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	spawn_helper(t_exec_data *ctx, t_parse_data *pd, t_env **env)
{
	int	res;

	res = pre_exec_prep(pd->commands[ctx->i], env, pd, ctx->pipe);
	if (res == -1)
		parent_run(pd->commands[ctx->i], &ctx->fd, ctx->pipe);
	return (res);
}

static void	spawn_fork(t_exec_data *ctx, t_parse_data *pd, \
			t_env **env, pid_t *pids)
{
	pids[ctx->i] = fork();
	if (pids[ctx->i] < 0)
		exit(1);
	if (pids[ctx->i] == 0)
		handle_child_process(pd->commands[ctx->i], ctx->fd, env, ctx->pipe);
	handle_parent_process(pd->commands[ctx->i], &ctx->fd, ctx->pipe);
}

///NOTE: This function must be only use to help exit_code calls
// 0 on sucess
// -1 on failure
int	exit_code_helper(t_parse_data *pd, t_env **env)
{
	char	*exit_str;
	int		rc;

	exit_str = ft_itoa(pd->pd_exit_status);
	if (!exit_str)
		return (-1);
	rc = replace_env_value(env, "?", exit_str);
	free (exit_str);
	return (rc);
}

int	spawn_processes(t_parse_data *pd, t_env **env, pid_t *pids)
{
	t_exec_data	ctx;

	ctx.fd = -1;
	ctx.i = 0;
	ctx.pipe[0] = -1;
	ctx.pipe[1] = -1;
	pd->n_spawn_pids = 0;
	while (ctx.i < pd->n_cmds)
	{
		ctx.rc = spawn_helper(&ctx, pd, env);
		if (ctx.rc == -1)
			break ;
		if (ctx.rc == 1)
			exit(1);
		spawn_fork(&ctx, pd, env, pids);
		ctx.i++;
		pd->n_spawn_pids++;
	}
	if (ctx.fd != -1)
		close(ctx.fd);
	return (0);
}
