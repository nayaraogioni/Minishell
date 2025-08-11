/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_var.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 17:46:12 by nayara            #+#    #+#             */
/*   Updated: 2025/07/19 01:48:37 by dopereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/libft.h"
#include "minishell.h"
#include <time.h>

void	update_last_bg_pid(t_lexer *lexer, pid_t pid)
{
	lexer->last_bg_pid = pid;
}

char	*get_special_var(char *var_name, t_lexer *lexer)
{
	if (ft_strcmp(var_name, "?") == 0)
		return (ft_itoa(lexer->exit_status));//Modify here
	else if (ft_strcmp(var_name, "$") == 0)
		return (ft_itoa(getppid()));
	else if (ft_strcmp(var_name, "0") == 0)
		return (ft_strdup("minishell"));
	else if (ft_strcmp(var_name, "#") == 0)
		return (ft_strdup("0"));
	else if (ft_strcmp(var_name, "*") == 0)
		return (ft_strdup(""));
	else if (ft_strcmp(var_name, "@") == 0)
		return (ft_strdup(""));
	else if (ft_strcmp(var_name, "!") == 0)
	{
		if (lexer->last_bg_pid > 0)
			return (ft_itoa(lexer->last_bg_pid));
		else
			return (ft_strdup(""));
	}
	return (NULL);
}

int	set_value(char *var_value, char	*var_name, t_lexer *lexer, t_env *my_env)
{
	var_value = get_special_var((char *)var_name, lexer);
	if (!var_value)
		var_value = ft_getenv(my_env, var_name);
	if (!var_value)
		var_value = ft_strdup("");
	if (!var_value)
		return (-1);
	return (0);
}

int	expand_variables(t_lexer *lexer, t_env *my_env)
{
	int		i;
	char	*var_name;
	char	*var_value;
	char	*old_text;

	i = 0;
	while (i < lexer->token_count)
	{
		if (lexer->tokens[i].type == T_VAR)
		{
			var_name = lexer->tokens[i].text + 1;
			var_value = get_special_var((char *)var_name, lexer);
			if (!var_value)
				var_value = ft_getenv(my_env, var_name);
			if (!var_value)
				var_value = ft_strdup("");
			if (!var_value)
				return (-1);
			old_text = lexer->tokens[i].text;
			lexer->tokens[i].text = ft_strdup(var_value);
			free(old_text);
			lexer->tokens[i].type = T_WORD;
		}
		i++;
	}
	return (0);
}

char	*expand_heredoc_line(char *line, t_env *env)
{
	int		i;
	int		start;
	char	*key;
	char	*out = ft_strdup("");

	i = 0;
	while (line[i])
	{
		if (line[i] == '$')
		{
			start = i++;
			key = NULL;
			if (line[i] == '(')
			{
				i++; // skip '('
				size_t name_start = i;
				while (line[i] && (ft_isalnum((unsigned char)line[i]) || line[i] == '_'))
					i++;
				if (line[i] == ')')
				{
					key = ft_substr(line, name_start, i - name_start);
					i++; // skip ')'
				}
				else
				{
					// no closing ')': treat literally
					i = start + 1;
					//free(key);
					//key = NULL;
				}
			}
			else
			{
				size_t name_start = i;
				while (line[i] && (isalnum((unsigned char)line[i]) || line[i]=='_'))
					i++;
				key = ft_substr(line, name_start, i - name_start);
			}
			if (key)
			{
				char	*raw_value = ft_getenv(env, key);
				char	*val;

				if (raw_value)
					val = ft_strdup(raw_value);
				else
					val = ft_strdup("");
				//if (!val)
					//val = ft_strdup("");  // undefined vars expand to empty
				// Join: free only 'out', keep 'val' alive so we can free it ourselves
				out = ft_strjoin_free(out, val, 'L');//R
				free(val);
				free(key);
				continue ;
			}
			// If we fell through (bad syntax), emit literal '$'
			out = ft_strjoin_free(out, "$", 'L');//N
		}
		else
		{
			// copy a single character
			char tmp[2] = { line[i], '\0' };
			out = ft_strjoin_free(out, tmp, 'L');//R
			i++;
		}
	}
	return (out);
}
