/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nayara <nayara@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 23:08:53 by dopereir          #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2025/09/09 17:24:47 by nayara           ###   ########.fr       */
=======
/*   Updated: 2025/09/09 14:57:29 by dopereir         ###   ########.fr       */
>>>>>>> 5fe75188209e5c772b3fc472f083b677515b9b15
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H
# include "lexer.h"
# define MAX_ARGS 1024

typedef struct s_command
{
	t_token_type		type; // Type of command
	char				*name; // Name of the command (e.g., "ls", "echo")
	char				*path; // generate path w/ cmd_path_generator()
	char				*input_file; // Input file for redirection
	char				*output_file; // Output file for redirection
	char				*argv[MAX_ARGS]; // Arguments for the command
	char				*hd_delim; //delimiter to use on heredoc
<<<<<<< HEAD
	int					hd_expand; // flag 1 -> expand 0 -> n expande
=======
	int					hd_expand_flag;
>>>>>>> 5fe75188209e5c772b3fc472f083b677515b9b15
	int					heredoc_fd;
	pid_t				pid_filename_output;
	// para pipelines e sequencias parser phase
	struct s_command	**commands; // arrray de comandos filhos
	int					command_count;
	struct s_command	*left;
	struct s_command	*right;
	int					next_is_pipe;
	int					next_is_and;
}			t_command;

typedef struct s_pbuilder
{
	int			i;
	int			arg_index;
	char		*acc;
	int			was_append;
	int			args_count;
	int			res;
}			t_pbuilder;

typedef struct s_pipe_data
{
	int			start;
	int			pipe_pos;
	int			i;
	t_lexer		*sublexer;
	t_command	*leaf;
}			t_pipe_data;

typedef struct s_seq_data
{
	int		start;
	int		op_pos;
	int		i;
	t_lexer	*sublexer;
}			t_seq_data;

typedef struct s_sub_data
{
	t_lexer	*sublexer;
	int		i;
	int		j;
}			t_sub_data;

//struct to use in token_counter function
typedef struct s_token_counter
{
	char	*s;
	int		res;
	char	quote_char;
}			t_token_counter;

//struct to use in process_single_token() function
typedef struct s_proc_token
{
	char	*start;
	char	*tok_begin;
	int		len;
	int		qt_flag;
	int		join_prev;
}			t_proc_token;

typedef struct s_token_loop
{
	char	*s;
	int		i;
	int		rc;
}			t_token_loop;

#endif
