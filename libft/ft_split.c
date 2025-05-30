/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/25 17:00:46 by dopereir          #+#    #+#             */
/*   Updated: 2024/05/27 19:28:18 by dopereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

static	size_t	count_helper(char const *s, char c)
{
	size_t	count;

	count = 0;
	while (*s)
	{
		while (*s == c)
			s++;
		if (*s)
		{
			count++;
			while (*s && *s != c)
				s++;
		}
	}
	return (count);
}

static void	*ft_free(char **s, int count)
{
	auto int i = 0;
	while (i < count)
	{
		free(s[i]);
		i++;
	}
	free(s);
	return (NULL);
}

static char	*allocate_word(char const *s, size_t start, size_t end)
{
	char	*word;

	word = malloc((end - start + 1) * sizeof(char));
	if (!word)
		return (NULL);
	ft_strlcpy(word, s + start, end - start + 1);
	return (word);
}

char	**ft_split(char const *s, char c)
{
	char	**new_string;
	size_t	start;

	auto size_t i = 0;
	auto size_t j = 0;
	new_string = malloc((count_helper(s, c) + 1) * sizeof(char *));
	if (!new_string)
		return (NULL);
	while (i < count_helper(s, c))
	{
		while (s[j] == c)
			j++;
		start = j;
		while (s[j] && s[j] != c)
			j++;
		new_string[i] = allocate_word(s, start, j);
		if (!new_string[i])
			return (ft_free(new_string, i));
		i++;
	}
	new_string[i] = NULL;
	return (new_string);
}

/* #include <stdio.h>

int main(void)
{
	char	**split;
	char const *s = "Hello, World! This is a test.";
	char	c = ' ';
	int	i;

	split = ft_split(s, c);
	if (!split)
	{
		printf("Error: Memory allocation failed.\n");
		return (1);
	}

	for (i = 0; split[i] != NULL; i++)
	{
		printf("split[%d] = %s\n", i, split[i]);
	}

	ft_free(split, i);

	return (0);
}
 */