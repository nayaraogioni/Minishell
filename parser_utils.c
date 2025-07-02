/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 17:22:24 by dopereir          #+#    #+#             */
/*   Updated: 2025/06/30 22:49:18 by dopereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "minishell.h"

int	has_pipes(t_lexer *lexer)
{
	int	i;

	i = 0;
	while (i < lexer->token_count)
	{
		if (lexer->tokens[i].type == T_PIPE)
			return (1);
		i++;
	}
	return (0);
}

int	has_logical_operators(t_lexer *lexer)
{
	int	i;

	i = 0;
	while (i < lexer->token_count)
	{
		if (lexer->tokens[i].type == T_AND)
			return (1);
		i++;
	}
	return (0);
}

// conta quantos argumentos sao palavras (T_WORD) e nao redirecionamentos
int	count_args(t_lexer *lexer)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (i < lexer->token_count)
	{
		if (lexer->tokens[i].type == T_WORD)
		{
			count++;
			i++;
		}
		else if (lexer->tokens[i].type == T_REDIR_IN || lexer->tokens[i].type == T_REDIR_OUT || lexer->tokens[i].type == T_REDIR_APPEND || lexer->tokens[i].type == T_REDIR_HEREDOC)
		{
			i++;
			if (i < lexer->token_count && lexer->tokens[i].type == T_WORD)
				i++;
		}
		else
			i++;
	}
	return (count);
}
void free_command(t_command *cmd)
{
    int i;

    if (!cmd)
        return;

    // Free basic string fields
    if (cmd->name)
    {
        free(cmd->name);
        cmd->name = NULL;
    }
    if (cmd->path)
    {
        free(cmd->path);
        cmd->path = NULL;
    }
    if (cmd->input_file)
    {
        free(cmd->input_file);
        cmd->input_file = NULL;
    }
    if (cmd->output_file)
    {
        free(cmd->output_file);
        cmd->output_file = NULL;
    }
    if (cmd->filename)
    {
        free(cmd->filename);
        cmd->filename = NULL;
    }

    // Free argv array
    i = 0;
    while (i < MAX_ARGS && cmd->argv[i])
    {
        free(cmd->argv[i]);
        cmd->argv[i] = NULL;
        i++;
    }

    // Free commands array
    if (cmd->commands)
    {
        i = 0;
        while (i < cmd->command_count && i < MAX_ARGS)
        {
            if (cmd->commands[i])
            {
                free_command(cmd->commands[i]);
                cmd->commands[i] = NULL;
            }
            i++;
        }
        free(cmd->commands);
        cmd->commands = NULL;
    }

    // Free left and right commands (for tree structure)
    if (cmd->left)
    {
        free_command(cmd->left);
        cmd->left = NULL;
    }
    if (cmd->right)
    {
        free_command(cmd->right);
        cmd->right = NULL;
    }

    free(cmd);
}

int	find_next_pipe(t_lexer *lexer, int start)
{
	int	i;

	i = start;
	while (i < lexer->token_count)
	{
		if (lexer->tokens[i].type == T_PIPE)
			return (i);
		i++;
	}
	return (-1); // se nao encontrar
}

int	find_next_logical_operator(t_lexer *lexer, int start)
{
	int	i;

	i = start;
	while (i < lexer->token_count)
	{
		if (lexer->tokens[i].type == T_AND)
			return (i);
		i++;
	}
	return (-1); // se nao encontrar
}

t_lexer	*create_sublexer(t_lexer *lexer, int start, int end)
{
	t_lexer	*sublexer;
	int		i;
	int		j;

	sublexer = malloc(sizeof(t_lexer));
	if (!sublexer)
		return (NULL);
	sublexer->input = lexer->input; // assume que o input eh o mesmo
	sublexer->token_count = end - start;
	if (sublexer->token_count <= 0)
	{
		free(sublexer);
		return (NULL);
	}
	sublexer->tokens = malloc(sizeof(t_token) * sublexer->token_count);
	if (!sublexer->tokens)
	{
		free(sublexer);
		return (NULL);
	}
	j = 0;
	i = start;
	while (i < end)
	{
		sublexer->tokens[j].type = lexer->tokens[i].type;
		sublexer->tokens[j].text = ft_strdup(lexer->tokens[i].text);
		if (!sublexer->tokens[j].text)
		{
			while (j > 0)
			{
				j--;
				free(sublexer->tokens[j].text);
			}
			free(sublexer->tokens);
			free(sublexer);
			return (NULL);
		}
		j++;
		i++;
 	}
	return (sublexer);
}

void	free_sublexer(t_lexer *sublexer)
{
	int	i;

	if (!sublexer)
		return;
	if (sublexer->tokens)
	{
		i = 0;
		while (i < sublexer->token_count)
		{
			if (sublexer->tokens[i].text)
				free(sublexer->tokens[i].text);
			i++;
		}
		free(sublexer->tokens);
	}
	free(sublexer);
}
