/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_commands.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 22:48:16 by dopereir          #+#    #+#             */
/*   Updated: 2025/07/04 10:08:06 by dopereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "minishell.h"
#include "parser.h"
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

void	exec_parsed_cmds(t_parse_data *pd)
{
	int		prev_fd;
	pid_t	pids[MAX_ARGS];
	int		curr_pipe[2];
	int		i;
	int		heredoc_fd;
	int		make_pipe;
	t_command	*cmd;

	prev_fd = -1;
	for (i = 0; i < pd->n_cmds; i++)
	{
		cmd = pd->commands[i];
		make_pipe = cmd->next_is_pipe;
		if (make_pipe && pipe(curr_pipe) < 0)
		{
			printf("Error on set_pipe\n");
			exit(1);
		}
		pids[i] = fork();//FORK THE CHILD
		if (pids[i] < 0)
		{
			perror("fork at exec_commands");
			exit(1);
		}
		if (pids[i] == 0) //CHILD RUNTIME
		{
			//input setting
			if (cmd->hd_delim) //IS HEREDOC?
			{
				heredoc_fd = set_heredoc(cmd->hd_delim);
				if (heredoc_fd < 0)
					exit(1);
				dup2(heredoc_fd, STDIN_FILENO);
				close(heredoc_fd);
			}
			else if (cmd->input_file) //SETUP STDIN
				set_input(cmd);
			else if (prev_fd != -1)
			{
				dup2(prev_fd, STDIN_FILENO);
				close(prev_fd);
			}

			//output setting
			if (cmd->output_file) //SETUO STDOUT
				set_output(cmd);
			else if (make_pipe)
				dup2(curr_pipe[1], STDOUT_FILENO);
			if (make_pipe)
			{
				close(curr_pipe[0]);
				close(curr_pipe[1]);
			}
			
			
			cmd->path = cmd_path_generator(cmd->name);
			execve(cmd->path, cmd->argv, environ);
			free (cmd->path);
			perror("execve failed");// only if exec fails
			exit(127);
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
