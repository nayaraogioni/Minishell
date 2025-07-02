/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_commands.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 22:48:16 by dopereir          #+#    #+#             */
/*   Updated: 2025/06/25 22:46:14 by dopereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//#include "lexer.h"
#include "lexer.h"
#include "minishell.h"
#include "parser.h"
#include <unistd.h>
//#include "parser.h"

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
	int		pipe_read;
	int		pipe_write;
	int		i;
	int		heredoc_fd;
	t_command	*cmd;

	for (i = 0; i < pd->n_cmds; i++)
	{
		cmd = pd->commands[i];
		pipe_read = -1;
		pipe_write = -1;
		prev_fd = -1;
		if (cmd->next_is_pipe == 1)
		{
			if (set_pipe(&pipe_read, &pipe_write) < 0)
			{
				printf("Error on set_pipe\n");
				exit(1);
			}
		}
		pids[i] = fork();//FORK THE CHILD
		if (pids[i] < 0)
		{
			perror("fork at exec_commands");
			exit(1);
		}
		if (pids[i] == 0) //CHILD RUNTIME
		{
			if (cmd->type == T_REDIR_HEREDOC) //IS HEREDOC?
			{
				//add a heredoc_delim variable to parser to keep track of the delim str?
				heredoc_fd = set_heredoc(cmd->hd_delim);//DELIMITER)
				if (heredoc_fd < 0)
					exit(1);
				dup2(heredoc_fd, STDIN_FILENO);
				close(heredoc_fd);
			}
			else if (cmd->input_file) //SETUP STDIN
				set_input(cmd);
			else if (prev_fd != -1)
				dup2(prev_fd, STDIN_FILENO);
			if (cmd->output_file) //SETUO STDOUT
				set_output(cmd);
			else if (pipe_write != -1)
				dup2(pipe_write, STDOUT_FILENO);
			if (prev_fd != -1) // CLOSE FDS WE DONT NEED
				close(prev_fd);
			if (pipe_read  != -1)
				close(pipe_read);
			if (pipe_write != -1)
				close(pipe_write);
			cmd->path = cmd_path_generator(cmd->name);
			execve(cmd->path, cmd->argv, environ);
			free (cmd->path);
			perror("execve failed");// only if exec fails
			exit(127);
		}
		//PARENT RUNTIME
		if (prev_fd != -1)
			close(prev_fd);
		if (pipe_write != -1)
			close(pipe_write);
		prev_fd = pipe_read; //NEXT INTERATION CHILDS READ FROM HERE
	}
	for (i = 0; i < pd->n_cmds; i++)
		waitpid(pids[i], NULL, 0);
}
