/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_var_heredoc_helper.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 22:30:39 by dopereir          #+#    #+#             */
/*   Updated: 2025/08/07 22:30:55 by dopereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*hd_helper_extract_key(char *line, int *i)
{
	char	*key;
	int		name_start;

	key = NULL;
	(*i)++;
	if (line[*i] == '(')
	{
		name_start = ++(*i);
		while (line[*i] && (ft_isalnum(line[*i]) || line[*i] == '_'))
			(*i)++;
		if (line[*i] == ')')
			key = ft_substr(line, name_start, (*i)++ - name_start);
	}
	else
	{
		name_start = *i;
		while (line[*i] && (ft_isalnum(line[*i]) || line[*i] == '_'))
			(*i)++;
		key = ft_substr(line, name_start, *i - name_start);
	}
	return (key);
}

char	*hd_helper_getvalue(char *key, t_env *env)
{
	char	*raw_value;
	char	*value;

	raw_value = ft_getenv(env, key);
	if (raw_value)
		value = ft_strdup(raw_value);
	else
		value = ft_strdup("");
	return (value);
}

char	*hd_helper_exp_varname(char *out, char *key, t_env *env)
{
	char	*value;
	char	*new_out;

	value = hd_helper_getvalue(key, env);
	new_out = ft_strjoin_free(out, value, 'L');
	free(value);
	free(key);
	return (new_out);
}

char	*hd_helper_append_char(char *out, char c)
{
	char	tmp[2];
	char	*new_out;

	tmp[0] = c;
	tmp[1] = '\0';
	new_out = ft_strjoin_free(out, tmp, 'L');
	return (new_out);
}
