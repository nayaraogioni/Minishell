/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 17:45:19 by dopereir          #+#    #+#             */
/*   Updated: 2024/05/25 18:19:47 by dopereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdio.h>

char	*ft_strchr(const char *s, int c)
{
	while (*s)
	{
		if (*s == (char)c)
			return ((char *)s);
		s++;
	}
	if ((char)c == '\0')
		return ((char *)s);
	return (NULL);
}

/* int	main()
{
	const char	test[] = "teste";
	char	*c;

	c = ft_strchr(test, 'a');

	if (c != NULL)
	{
		printf("Character found at position %ld\n", c - test + 1);
		printf("%p\n", c - test + 1);
	}
	else
		printf("Character not found in the string.\n");
	return (0);
} */
