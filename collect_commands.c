/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   collect_commands.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 08:48:55 by dopereir          #+#    #+#             */
/*   Updated: 2025/09/09 15:34:20 by dopereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"

static void	copy_simple_fields(t_command *src, t_command *dest)
{
	dest->type = src->type;
	dest->next_is_pipe = src->next_is_pipe;
	dest->next_is_and = src->next_is_and;
	if (src->name)
		dest->name = ft_strdup(src->name);
	if (src->path)
		dest->path = ft_strdup(src->path);
	if (src->input_file)
		dest->input_file = ft_strdup(src->input_file);
	if (src->output_file)
		dest->output_file = ft_strdup(src->output_file);
	if (src->hd_delim)
		dest->hd_delim = ft_strdup(src->hd_delim);
	if (src->pid_filename_output)
		dest->pid_filename_output = src->pid_filename_output;
}

static t_command	*copy_simple_cmd(t_command *src)
{
	t_command	*dest;
	int			i;

	dest = init_command();
	i = 0;
	if (!dest)
		return (NULL);
	copy_simple_fields(src, dest);
	while (i < MAX_ARGS && src->argv[i])
	{
		dest->argv[i] = ft_strdup(src->argv[i]);
		i++;
	}
	dest->argv[i] = NULL;
	return (dest);
}

static void	collect_commands(t_command *cmd, t_parse_data *parsed_data)
{
	t_command	*copy;
	int			i;

	if (!cmd)
		return ;
	i = 0;
	while (i < cmd->command_count)
	{
		collect_commands(cmd->commands[i], parsed_data);
		i++;
	}
	if (cmd->left)
		collect_commands(cmd->left, parsed_data);
	if (cmd->right)
		collect_commands(cmd->right, parsed_data);
	if (cmd->command_count == 0 && !cmd->left && !cmd->right)
	{
		if (parsed_data->n_cmds < MAX_ARGS)
		{
			copy = copy_simple_cmd(cmd);
			if (copy)
				parsed_data->commands[parsed_data->n_cmds++] = copy;
		}
	}
}

void	free_parsed_data(t_parse_data *parsed_data)
{
	int	i;

	i = 0;
	while (i < parsed_data->n_cmds && i < MAX_ARGS)
	{
		if (parsed_data->commands[i])
		{
			free_command(parsed_data->commands[i]);
			parsed_data->commands[i] = NULL;
		}
		i++;
	}
	parsed_data->n_cmds = 0;
	parsed_data->n_spawn_pids = 0;
}

// > format_parsed_data(t_lexer *lexer)
// copy the data returned by parse_function into a t_parse_data structure,
// and free the original data
t_parse_data	format_parsed_data(t_lexer *lexer, t_env *my_env)
{
	t_parse_data	parsed_data;
	t_command		*root;
	int				i;

	parsed_data.n_cmds = 0;
	parsed_data.pd_exit_status = 0;
	i = 0;
	while (i < MAX_ARGS)
		parsed_data.commands[i++] = NULL;
	root = parse_function(lexer, my_env);
	if (!root)
		return (parsed_data);
	collect_commands(root, &parsed_data);
	free_command(root);
	return (parsed_data);
}

/* ------------ TESTING FUNCTIONS -------------*/

/*static const char	*type_to_string(t_token_type type)
{
	switch (type) {
		case T_WORD:			return "WORD";
		case T_PIPE:			return "PIPE";
		case T_REDIR_IN:		return "REDIR_IN";
		case T_REDIR_OUT:		return "REDIR_OUT";
		case T_REDIR_APPEND:	return "REDIR_APPEND";
		case T_REDIR_HEREDOC:	return "REDIR_HEREDOC";
		case T_AND:				return "AND";
		default:				return "UNKNOWN";
	}
}

void	print_parsed_data(const t_parse_data *pd)
{
	for (int i = 0; i < pd->n_cmds; ++i) {
		const t_command *cmd = pd->commands[i];
		printf("---- Command #%d ----\n", i);
		printf("Type: %s\n", type_to_string(cmd->type));
		printf("Name: %s\n", cmd->name ? cmd->name : "(null)");
		printf("Path: %s\n", cmd->path ? cmd->path : "(null)");
		printf("Input Redir: %s\n",
			cmd->input_file ? cmd->input_file : "(none)");
		printf("Output Redir: %s\n",
			cmd->output_file ? cmd->output_file : "(none)");
		printf("Heredoc Delimiter: %s\n",
			cmd->hd_delim ? cmd->hd_delim : "(none)");
		printf("Pipe flag: %d\n", cmd->next_is_pipe);
		printf("Heredoc FD: %d\n", (int)cmd->heredoc_fd);

		// Print argv[]
		printf("Arguments:");
		if (!cmd->argv[0]) {
			printf(" (none)\n");
		} else {
			for (int a = 0; a < MAX_ARGS && cmd->argv[a]; ++a) {
				printf(" [%s]", cmd->argv[a]);
			}
			printf("\n");
		}

		// If there were sub‑commands (shouldn't happen for leaves)
		if (cmd->command_count > 0) {
			printf(" (Unexpected subcmds count = %d)\n", cmd->command_count);
		}
		printf("\n");
	}
	}*/
