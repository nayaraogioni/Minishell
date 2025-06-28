/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nayara <nayara@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 23:08:53 by dopereir          #+#    #+#             */
/*   Updated: 2025/06/28 16:43:40 by nayara           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H
# include "lexer.h"
# include "minishell.h"
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
	char			*name; // Name of the command (e.g., "ls", "echo")
	char			*path; // Full path to the command executable
	char			*input_file; // Input file for redirection
	char			*output_file; // Output file for redirection
	char			*argv[MAX_ARGS]; // Arguments for the command
	char			*filename; //path to file redirection
	pid_t			pid_filename_output;
	// para pipelines e sequencias
	struct s_command	**commands; // arrray de comandos filhos
	int	command_count;
	struct s_command	*left;
	struct s_command	*right;
	int	next_is_pipe;
}			t_command;


#endif
