/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 23:08:53 by dopereir          #+#    #+#             */
/*   Updated: 2025/06/25 22:37:42 by dopereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H
# include "lexer.h"
# include "minishell.h"

typedef struct s_redirect
{
	t_token_type	type;
	char			*filename;
	int				fd;	// File descriptor for the redirection
}			t_redirect;

typedef struct s_command //mkdir test argv[0]
{
	t_token_type		type; // Type of command (e.g., simple, pipeline)
	char			*name; // Name of the command (e.g., "ls", "echo")
	char			*path; // Full path to the command executable
	char			*input_file; // Input file for redirection
	char			*output_file; // Output file for redirection
	char			*argv[MAX_ARGS]; // Arguments for the command
	char			*filename; //path to file redirection
	//pid_t			pid_filename_output;
}			t_command;


#endif
