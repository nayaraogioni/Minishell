/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment_functions_utils2.c                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 22:11:31 by dopereir          #+#    #+#             */
/*   Updated: 2025/08/20 18:49:34 by dopereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/libft.h"
#include "minishell.h"
#include <string.h>
#include <time.h>

///NOTE: This function handles a specific case to handle in export
//NOTE: this function must be called in parse_function (parser.c)
// Purpose: it sets a flag to indicate if the parser function should expand
// environmental variables or not, depends if the outer quotes is a single quote
int	export_exception_flag(t_lexer *lexer)
{
	char	*eq;
	int		flag;

	flag = 0;
	if (lexer->token_count > 0 && lexer->tokens[0].text &&
		ft_strcmp(lexer->tokens[0].text, "export") == 0)
	{
		if (lexer->token_count > 1 && lexer->tokens[1].text)
		{
			eq = ft_strchr(lexer->tokens[1].text, '=');
			if (eq && *(++eq) == '\'')
					flag = 1;
		}
	}
	return (flag);
}

//This function assumes that value[0] is either " or '
//Make sure of that.
char	*trim_raw_value(char *value)
{
	int		len;
	char	*start;
	char	*end;
	char	*result;
	char	quote_type;

	if (!value)
		return (NULL);
	quote_type = value[0];
	start = ft_strchr(value, quote_type);
	if (!start)
		return (NULL);
	start++;
	if (*(start - 1) == quote_type)
		end = ft_strchr(start, quote_type);
	else
		end = ft_strchr(start, quote_type);
	if (!end)
		return (NULL);

	len = end - start;
	result = malloc(len + 1);
	if (!result)
		return (NULL);
	ft_strncpy(result, start, len);
	result[len] = '\0';
	return (result);
}

//this function will be called when we find a double quote inside a
char	*literal_argv_expander(char *eq, char **argv, int *i)
{
	int		j = 2;
	char	*tmp;
	char	*res;
	int		klen;
	char	*key;

	klen = eq - argv[*i];
	key = ft_strndup(argv[*i], klen);
	if (!key)
		return (NULL);
	res = ft_strdup(eq + 1);
	if (!res)
		return (free(key), NULL);
	while (argv[j])
	{
		tmp = join_words(res, " ");
		if (!tmp)
		{
			free (key);
			free (res);
			return (NULL);
		}
		res = tmp;
		tmp = join_words(res, argv[j]);
		if (!tmp)
		{
			free (key);
			free (res);
			return (NULL);
		}
		res = tmp;
		j++;
	}
	printf("AFTER JOIN_WORDS LOOP: %s\n", res);
	tmp = trim_raw_value(res);
	free(res);
	printf("AFTER trim_raw_value: %s\n", tmp);
	free (key);
	return (tmp);
}
