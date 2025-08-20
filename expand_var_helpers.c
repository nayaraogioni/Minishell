/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_var_helpers.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 22:22:43 by dopereir          #+#    #+#             */
/*   Updated: 2025/08/12 22:23:27 by dopereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/libft.h"
#include "minishell.h"
#include <time.h>

int	append_char(char **dst, char c)
{
	char	buf[2];
	char	*tmp;

	buf[0] = c;
	buf[1] = '\0';
	tmp = join_words(*dst, buf);
	if (!tmp)
		return (-1);
	*dst = tmp;
	return (0);
}

static int	envp_expander(char *src, int *i, char **out, t_env *env)
{
	int		j;
	char	*name;
	char	*val;
	char	*tmp;

	j = *i + 1;
	while (ft_isalnum(src[j]) || src[j] == '_')
		j++;
	name = ft_substr(src, *i + 1, j - (*i + 1));
	if (name)
		val = ft_getenv(env, name);
	else
		val = NULL;
	if (val)
		tmp = join_words(*out, val);
	else
		tmp = join_words(*out, "");
	free(name);
	if (!tmp)
		return (-1);
	*out = tmp;
	*i = j;
	return (0);
}

static int	process_raw_var_case(t_token *t, t_env *env, int *i, char **out)
{
	int	j;

	if (t->text[*i + 1] == '?')
	{
		if (!last_exit_expander(t->exp_exit_status, out))
			return (-1);
		*i += 2;
		return (0);
	}
	j = *i + 1;
	if (!(ft_isalpha(t->text[j]) || t->text[j] == '_'))
	{
		if (append_char(out, '$') < 0)
			return (-1);
		(*i)++;
		return (0);
	}
	if (envp_expander(t->text, i, out, env) < 0)
		return (-1);
	return (0);
}

static int	iter_helper(t_env *env, t_token *t, char **out, int *i)
{
	if (t->text[*i] == '\\')
	{
		t->exp_index_consumed = process_backslash(*i, out, t->text, t->quot);
		if (t->exp_index_consumed < 0)
			return (-1);
		(*i) += t->exp_index_consumed;
		return (0);
	}
	if (t->text[*i] == '$')
	{
		if (process_raw_var_case(t, env, i, out) < 0)
			return (-1);
		return (0);
	}
	if (append_char(out, t->text[*i]) < 0)
		return (-1);
	return (1);
}

char	*expand_word_text(t_env *env, t_token *t)
{
	int		i;
	char	*out;
	int		res;

	i = 0;
	out = ft_strdup("");
	if (!out)
		return (NULL);
	while (t->text[i])
	{
		res = iter_helper(env, t, &out, &i);
		if (res == -1)
		{
			free(out);
			return (NULL);
		}
		if (res == 0)
			continue ;
		i++;
	}
	return (out);
}
