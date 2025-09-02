/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtok.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 19:34:28 by dopereir          #+#    #+#             */
/*   Updated: 2025/09/02 16:29:16 by dopereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	helper(char **base, char **cursor, char *str)
{
	if (*base)
		free(*base);
	*base = ft_strdup(str);
	*cursor = *base;
}

long	check_overflow(long *r, int d, int b, int s)
{
	if (s == 1 && *r > (LONG_MAX - d) / b)
		return (LONG_MAX);
	if (s == -1 && (unsigned long)*r > ((unsigned long)LONG_MAX + 1 - d) / b)
		return (LONG_MIN);
	return (0);
}

char	*ft_strtok(char *str, char *sep)
{
	static char	*base = NULL;
	static char	*cursor = NULL;
	char		*token;

	if (str != NULL)
		helper(&base, &cursor, str);
	if (cursor == NULL)
		return (NULL);
	cursor += ft_strspn(cursor, sep);
	if (*cursor == '\0')
	{
		free(base);
		base = NULL;
		cursor = NULL;
		return (NULL);
	}
	token = cursor;
	cursor = cursor + ft_strcspn(cursor, sep);
	if (*cursor != '\0')
		*(cursor++) = '\0';
	return (token);
}
