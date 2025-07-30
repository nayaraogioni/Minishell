/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 23:08:53 by dopereir          #+#    #+#             */
/*   Updated: 2025/06/28 17:26:58 by dopereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H
# include "lexer.h"
# define MAX_ARGS 1024

typedef struct s_redirect
{
	t_token_type	type;
	char			*filename;
	int				fd;	// File descriptor for the redirection
}			t_redirect;

typedef struct s_command //mkdir test argv[0]
{
	t_token_type		type; // Type of command (T_WORD, T_PIPE, T_REDIR_IN, T_REDIR_OUT, T_REDIR_APPEND, T_REDIR_HEREDOC, T_AND)
	char				*name; // Name of the command (e.g., "ls", "echo")
	char				*path; // generate path with cmd_path_generator(cmd->name)
	char				*input_file; // Input file for redirection
	char				*output_file; // Output file for redirection
	char				*argv[MAX_ARGS]; // Arguments for the command
	char				*filename; //path to file redirection
	char				*hd_delim; //delimiter to use on heredoc
	int					heredoc_fd;
	pid_t				pid_filename_output;
	// para pipelines e sequencias
	struct				s_command	**commands; // arrray de comandos filhos
	int					command_count;
	struct				s_command	*left;
	struct				s_command	*right;
	int					next_is_pipe;
	int					next_is_and;
}			t_command;


#endif
