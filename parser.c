/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nayara <nayara@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/07 14:37:35 by nayara            #+#    #+#             */
/*   Updated: 2025/06/07 14:37:37 by nayara           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


//#include "lexer.h"
#include "lexer.h"
#include "libft/libft.h"
#include "minishell.h"
#include <stdio.h>

t_command *init_command(void)
{
	t_command	*cmd;
	int			i;

	cmd = malloc(sizeof(t_command));
	if (!cmd)
		return NULL;
	cmd->type = T_WORD;
	cmd->name = NULL;
	cmd->path = NULL;
	cmd->input_file = NULL;
	cmd->output_file = NULL;
	cmd->filename = NULL;
	cmd->pid_filename_output = 0;
	cmd->command_count = 0;
	cmd->commands = NULL;
	cmd->left = NULL;
	cmd->right = NULL;
	cmd->next_is_pipe = 0;
	cmd->next_is_and = 0;
	cmd->hd_delim = NULL;
	i = 0;
	while (i < MAX_ARGS)
	{
		cmd->argv[i] = NULL;
		i++;
	}
	return (cmd);
}

t_command	*parse_simple_command(t_lexer *lexer)
{
	//printf("**** ENTER PARSE_SIMPLE_CMD ******\n");
	t_command	*cmd;
	int			args_count;
	int			i;
	int			arg_index;

	i = 0;
	arg_index = 0;
	cmd = init_command();
	if (!cmd)
		return NULL;
	args_count = count_args(lexer);
	if (args_count == 0)
	{
		free_command(cmd);
		return NULL;
	}
	while (i < lexer->token_count)
	{
		if (lexer->tokens[i].type == T_WORD)
		{
			if (cmd->name == NULL)
			{
				cmd->name = ft_strdup(lexer->tokens[i].text);
				if (!cmd->name)
				{
					free_command(cmd);
					return NULL;
				}
			}
			if (arg_index < MAX_ARGS - 1) // evita overflow
			{
				cmd->argv[arg_index] = ft_strdup(lexer->tokens[i].text);
				if (!cmd->argv[arg_index])
				{
					free_command(cmd);
					return NULL;
				}
				arg_index++;
			}
		}
		else if (lexer->tokens[i].type == T_REDIR_IN)
		{
			i++;
			if (i < lexer->token_count && lexer->tokens[i].type == T_WORD)
			{
				if (cmd->input_file)
					free(cmd->input_file);
				cmd->input_file = ft_strdup(lexer->tokens[i].text);
				if (!cmd->input_file)
				{
					free_command(cmd);
					return NULL;
				}
			}
			else
			{
				if (lexer->tokens[i].type)
					argument_redirs_error(lexer->tokens[i].type);
				else
					printf("minishell: syntax error near unexpected token `newline'\n");
				free_command(cmd);
				return (NULL); // falta o nome do arquivo de entrada
			}
		}
		else if (lexer->tokens[i].type == T_REDIR_OUT || lexer->tokens[i].type == T_REDIR_APPEND)
		{
			i++;
			if (i < lexer->token_count && lexer->tokens[i].type == T_WORD)
			{
				if (cmd->output_file)
					free(cmd->output_file);
				cmd->output_file = ft_strdup(lexer->tokens[i].text);
				if (!cmd->output_file)
				{
					free_command(cmd);
					return NULL;
				}
				if (lexer->tokens[i - 1].type == T_REDIR_APPEND)
					cmd->type = T_REDIR_APPEND;
				else if (lexer->tokens[i - 1].type == T_REDIR_OUT)
					cmd->type = T_REDIR_OUT;
			}
			else
			{
				//must do to every t_token_type
				if (lexer->tokens[i].type)
					argument_redirs_error(lexer->tokens[i].type);
				else
					printf("minishell: syntax error near unexpected token `newline'\n");
				free_command(cmd);
				return NULL;
			}
		}
		else if (lexer->tokens[i].type == T_REDIR_HEREDOC)
		{
			i++;
			if (i < lexer->token_count && lexer->tokens[i].type == T_WORD)
			{
				if (cmd->hd_delim)
					free(cmd->hd_delim);
				cmd->hd_delim = ft_strdup(lexer->tokens[i].text);
				if (!cmd->hd_delim)
				{
					free_command(cmd);
					return (NULL);
				}
				cmd->type = T_REDIR_HEREDOC;
				//AT EXECUTION PHASE SET SIGNAL HANDLER TO END HEREDOC AS WELL ^C or ^D
			}
		}
		else if (lexer->tokens[i].type == T_AND)
		{
			i++;
			if (i < lexer->token_count && lexer->tokens[i].type == T_WORD) //check bounds
			{
				cmd->argv[arg_index] = NULL; // termina o array de args com NULL
				return (cmd);
			}
		}
		/*else if (lexer->tokens[i].type == T_PIPE)
		{
			printf("minishell: syntax error near unexpected token `newline'\n");
			free_command(cmd);
			return (NULL);
			}*/
		i++;
	}
	cmd->argv[arg_index] = NULL; // termina o array de args com NULL
	return (cmd);
}

t_command	*parse_pipeline(t_lexer *lexer)
{
	//printf("**** ENTER PARSE_PIPELINE ******\n");
	t_command	*pipeline_cmd;
	t_lexer		*sublexer;
	int		start;
	int		pipe_pos;
	int		i;

	pipeline_cmd = init_command();
	if (!pipeline_cmd)
		return NULL;
	pipeline_cmd->type = T_PIPE;
	pipeline_cmd->commands = malloc(sizeof(t_command *) * MAX_ARGS);
	if (!pipeline_cmd->commands)
	{
		free_command(pipeline_cmd);
		return NULL;
	}
	i = 0;
 	while (i < MAX_ARGS)
	{
		pipeline_cmd->commands[i] = NULL;
		i++;
	}
	start = 0;
	while (start < lexer->token_count)
	{
		pipe_pos = find_next_pipe(lexer, start);
		if (pipe_pos == -1)
			pipe_pos = lexer->token_count; // se nao encontrar pega o final
		sublexer = create_sublexer(lexer, start, pipe_pos);
		if (!sublexer)
		{
			printf("minishell: syntax error near unexpected token `|'\n");
			free_command(pipeline_cmd);
			return NULL;
		}
		{
			t_command	*leaf = parse_simple_command(sublexer);
			if (!leaf)
			{
				free_sublexer(sublexer);
				free_command(pipeline_cmd);
				return (NULL);
			}
			if (pipe_pos < lexer->token_count)
				leaf->next_is_pipe = 1;
			pipeline_cmd->commands[pipeline_cmd->command_count] = leaf;
		}
		pipeline_cmd->command_count++;
		free_sublexer(sublexer);
		if (pipe_pos == lexer->token_count)
			break; // se for o ultimo comando sai do loop
		start = pipe_pos + 1; // pula o pipe para o proximo comando
	}
	return pipeline_cmd;
}

t_command	*parse_sequence(t_lexer *lexer, t_env *my_env)
{
	//printf("**** ENTER PARSE_SEQUENCE ******\n");
	t_command	*sequence_cmd;
	t_lexer		*sublexer;
	int		start;
	int		and_pos;
	int		i;

	sequence_cmd = init_command();
	if (!sequence_cmd)
		return NULL;
	sequence_cmd->type = T_AND;
	sequence_cmd->commands = malloc(sizeof(t_command *) * MAX_ARGS);
	if (!sequence_cmd->commands)
	{
		free_command(sequence_cmd);
		return NULL;
	}
	i = 0;
	while (i < MAX_ARGS)
	{
		sequence_cmd->argv[i] = NULL;
		i++;
	}
	start = 0;
	while (start < lexer->token_count)
	{
		and_pos = find_next_logical_operator(lexer, start);
		if (and_pos == -1)
			and_pos = lexer->token_count; // se nao encontrar pega o final
		sublexer = create_sublexer(lexer, start, and_pos);
		if (!sublexer)
		{
			free_command(sequence_cmd);
			return NULL;
		}
		sequence_cmd->commands[sequence_cmd->command_count] = parse_function(sublexer, my_env);
		if (!sequence_cmd->commands[sequence_cmd->command_count])
		{
			free_sublexer(sublexer);
			free_command(sequence_cmd);
			return NULL;
		}
		sequence_cmd->command_count++;
		free_sublexer(sublexer);
		if (and_pos == lexer->token_count)
			break; // se for o ultimo comando sai do loop
		start = and_pos + 1; // pula o operador logico para o proximo
	}
	return sequence_cmd;
}

t_command	*parse_function(t_lexer *lexer, t_env *my_env)
{
	if (has_variables(lexer))
	{
		if (expand_variables(lexer, my_env) == -1)
		{
			printf("minishell: error expanding variables\n");
			return NULL;
		}
	}
	if (has_logical_operators(lexer))
		return parse_sequence(lexer, my_env);
	if (has_pipes(lexer))
		return parse_pipeline(lexer);
	else
		return parse_simple_command(lexer);
}
