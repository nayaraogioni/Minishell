/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment_functions_utils2.c                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 22:11:31 by dopereir          #+#    #+#             */
/*   Updated: 2025/08/18 22:11:37 by dopereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/libft.h"
#include "minishell.h"
#include <time.h>

//this function will be called when we find a double quote inside a
char	*literal_argv_expander(char *eq, char **argv, int *idx)
{
	//receive the list of argv[][]
	// [export] [VAR="THIS] [IS] [A] [TEST"]
	//               ^ (start here)
	// append the argv until in found the closing quote
	int	i = 2;
	int	j = 3;
	char	*raw;
	char	*tmp;
	int		klen;
	char	*key;
	char	*value;

	//[export] [VAR="THIS] [IS] [A] [TEST"] [TEST]
	//   0          1       2    3     4      5

	//get the key
	char	*key_idx = ft_strchr(argv[1], '=');
	if (!eq)
		return (NULL);
	klen = eq - argv[1];
	key = ft_strndup(argv[i], klen);
	// key = VAR

	//get the fix piece of value
	value = ft_strdup(key_idx + 1);
	if (!value)
		return (free(key), NULL);
	// value = "this

	char	*closing_quote;
	if (!ft_strchr(argv[j], '"') || !ft_strchr(argv[j], '\''))
	{

	}
	if (argv[j])
		tmp = join_words(value, argv[j]);//value got freed
	while (argv[i])
	{
		if (argv[j])
		{
			if (!tmp)
			{
				free (value);
				free (key);
				return (NULL);
			}
			free (raw);
			raw = ft_strdup(tmp);
			if (!raw)
			{
				free (tmp);
				free (key);
				return (NULL);
			}
		}
		else
		{
			//append the last
		}
		j++;
		i++;
		value = ft_strdup(argv[i]);
	}

}
