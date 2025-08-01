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

#include "lexer.h"
#include "libft/libft.h"
#include "minishell.h"
#include "parser.h"
#include <asm-generic/errno-base.h>
#include <errno.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>

int	argv_delimiter(char *arg)
{
	char	*target_tokens[6];
	int		i;

	target_tokens[0] = "&&";
	target_tokens[1] = ">";
	target_tokens[2] = "<<";
	target_tokens[3] = ">>";
	target_tokens[4] = "|";
	target_tokens[5] = NULL;
	i = 0;
	while (target_tokens[i] != NULL)
	{
		if (ft_strcmp(arg, target_tokens[i]) == 0)
			return (1);
		i++;
	}
	return (0);
}

void	exec_parsed_cmds(t_parse_data *pd, t_env **env_list)
{
	int		prev_fd;
	pid_t	pids[MAX_ARGS];
	int		curr_pipe[2];
	int		i;
	//int		heredoc_fd;
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
			//printf("Entered parent built in part, cmd->name: %s\n", cmd->name);
			break ;
		}
		if (make_pipe && pipe(curr_pipe) < 0)
		{
			//printf("Error on set_pipe\n");
			exit(1);
		}
		pids[i] = fork();//FORK THE CHILD
		if (pids[i] < 0)
		{
			//perror("fork at exec_commands");
			exit(1);
		}

		if (pids[i] == 0) //CHILD RUNTIME
		{
			//input setting
			/*if (cmd->hd_delim) //IS HEREDOC?
			{
				heredoc_fd = set_heredoc(cmd->hd_delim);//CAUSES CONFLICT W MAIN
				if (heredoc_fd < 0)
					exit(1);
				dup2(heredoc_fd, STDIN_FILENO);
				close(heredoc_fd);
			}*/
			if (cmd->heredoc_fd >= 0)
			{
				dup2(cmd->heredoc_fd, STDIN_FILENO);
				close(cmd->heredoc_fd);
			}
			else if (cmd->input_file) //SETUP STDIN
			{
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
				if (set_output(cmd) == -1)
					exit(1);
			}
			else if (make_pipe)
				dup2(curr_pipe[1], STDOUT_FILENO);
			if (make_pipe)
			{
				close(curr_pipe[0]);
				close(curr_pipe[1]);
			}

			//child-safe built in dispatcher or exec not builtin command
			if (!ft_strcmp(cmd->name, "env"))
			{
				ft_env(*env_list);
				exit(0);
			}
			//add for pwd and echo later.


			cmd->path = cmd_path_generator(cmd->name);
			char	*str_test = ft_strdup(cmd->name);
			child_env = env_to_array(*env_list);
			execve(cmd->path, cmd->argv, child_env);

			int	err = errno;
			free (cmd->path);
			free_env_array(child_env, list_lenght(*env_list));//WRITE LIST_LENGHT
			//perror("execve failed");// only if exec fails
			if (err == ENOENT)
			{
				printf("Command '%s' not found.\n", str_test);
				free (str_test);
				exit(127);
			}
			else if (err == EACCES)
			{
				printf("Permission denied: %s\n", str_test);
				free (str_test);
				exit(126);
			}
			else if (err == EFAULT)
			{
				printf("Error executing '%s': %s\n", str_test, strerror(err));
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
	for (i = 0; i < pd->n_cmds; i++)
		waitpid(pids[i], NULL, 0);
}
