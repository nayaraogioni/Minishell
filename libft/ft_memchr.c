/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/25 15:25:47 by dopereir          #+#    #+#             */
/*   Updated: 2024/04/25 16:02:49 by dopereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *str, int c, size_t n)
{
	size_t		i;
	const char	*s;

	i = 0;
	s = (const char *)str;
	while (i < n)
	{
		if (s[i] == (char)c)
		{
			return ((void *)(s + i));
		}
		i++;
	}
	return (NULL);
}
