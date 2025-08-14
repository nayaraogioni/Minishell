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

#include "minishell.h"
#include "parser.h"
#include <signal.h>
<<<<<<< HEAD
#include <unistd.h>
=======
>>>>>>> 0384cda6b8b7f354a1d6b8ffa81772123f696e80

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
	if (!ft_strcmp(cmd->name, "env"))
	{
		ft_env(*env);
		return (0);
	}
	cmd->path = cmd_path_generator(cmd->name, *env);
	tmp_cmd_name = ft_strdup(cmd->name);
	child_env = env_to_array(*env);
	execve(cmd->path, cmd->argv, child_env);
	free (cmd->path);
	free_env_array(child_env, list_lenght(*env));
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

void	exec_parsed_cmds(t_parse_data *pd, t_env **env)
{
	pid_t		pids[MAX_ARGS];
	t_exec_data	ctx;

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
			exit (child_run(pd->commands[ctx.i], ctx.fd, env, ctx.pipe));
		parent_run(pd->commands[ctx.i], &ctx.fd, ctx.pipe);
		ctx.i++;
		pd->n_spawn_pids++;
	}
	if (ctx.fd != -1)
		close(ctx.fd);
	exit_code(pd, env, pids);
}

/*void	exec_parsed_cmds(t_parse_data *pd, t_env **env_list)
{
	int		prev_fd;
	pid_t	pids[MAX_ARGS];
	int		curr_pipe[2];
	int		i;
	int		make_pipe;
	bool	is_parent_bt;
	char	**child_env;
	t_command	*cmd;

	prev_fd = -1;
	for (i = 0; i < pd->n_cmds; i++)
	{
		cmd = pd->commands[i];
		is_parent_bt = is_parent_builtin(cmd->name);
		make_pipe = cmd->next_is_pipe;
		if (is_parent_bt && pd->n_cmds == 1)
		{
			if (cmd->input_file)
				set_input(cmd);
			if (cmd->output_file)
				set_output(cmd);
			run_parent_built(cmd, env_list);
			break ;
		}
		if (make_pipe && pipe(curr_pipe) < 0)
		{
			exit(1);
		}
		pids[i] = fork();//FORK THE CHILD
		if (pids[i] < 0)
		{
			exit(1);
		}

		if (pids[i] == 0) //CHILD RUNTIME
		{
			//input setting
			if (cmd->heredoc_fd >= 0)
			{
				//printf("GOT HERE HEREDOC FD\n");
				dup2(cmd->heredoc_fd, STDIN_FILENO);
				close(cmd->heredoc_fd);
			}
			else if (cmd->input_file) //SETUP STDIN
			{
				//printf("GOT HERE input_file\n");
				if (set_input(cmd) == -1)
					exit(1);
			}
			else if (prev_fd != -1)
			{
				dup2(prev_fd, STDIN_FILENO);
				close(prev_fd);
			}
			//output setting
			if (cmd->output_file) //SETUP STDOUT
			{
				//printf("GOT HERE output_file\n");
				if (set_output(cmd) == -1)
					exit(1);
			}
			else if (make_pipe)
			{
				//printf("GOT DUP2\n");
				dup2(curr_pipe[1], STDOUT_FILENO);
			}
			if (make_pipe)
			{
<<<<<<< HEAD
				//printf("GOT CLOSE PIPEFD 0 AND 1\n");
=======
      //printf("GOT CLOSE PIPEFD 0 AND 1\n");
>>>>>>> 0384cda6b8b7f354a1d6b8ffa81772123f696e80
				close(curr_pipe[0]);
				close(curr_pipe[1]);
			}

			if (!ft_strcmp(cmd->name, "env"))
			{
				//printf("CALLED FT_ENV\n");
				ft_env(*env_list);
				exit(0);
			}
			//add for pwd and echo later.

			cmd->path = cmd_path_generator(cmd->name);
			char	*str_test = ft_strdup(cmd->name);
			child_env = env_to_array(*env_list);
			execve(cmd->path, cmd->argv, child_env);
<<<<<<< HEAD

=======
>>>>>>> 0384cda6b8b7f354a1d6b8ffa81772123f696e80
			free (cmd->path);
			free_env_array(child_env, list_lenght(*env_list));//WRITE LIST_LENGHT
			if (errno == ENOENT)
			{
				//printf("GOT IN ENOENT CHECK\n");
				printf("Command '%s' not found.\n", str_test);
				free (str_test);
				exit(127);
			}
			else if (errno == EACCES)
			{
				printf("Permission denied: %s\n", str_test);
				free (str_test);
				exit(126);
			}
			else if (errno == EFAULT)
			{
				printf("Error executing '%s': %s\n", str_test, strerror(errno));
				free (str_test);
				exit(1);
			}
			free(str_test);
		}
		//PARENT RUNTIME
		if (make_pipe)
			close(curr_pipe[1]);
		if (prev_fd != -1)
			close(prev_fd);
		prev_fd = make_pipe ? curr_pipe[0] : -1;
	}
	if (prev_fd != -1)
		close(prev_fd);
	int	wstatus;
	for (i = 0; i < pd->n_cmds; i++)
	{
		if (waitpid(pids[i], &wstatus, 0) < 0)
			continue ;
		if (WIFEXITED(wstatus))
			pd->pd_exit_status = WEXITSTATUS(wstatus);
		else if (WIFSIGNALED(wstatus))
			pd->pd_exit_status = 128 + WTERMSIG(wstatus);
	}
	ft_setenv(env_list, "?", ft_itoa(pd->pd_exit_status));
}*/
