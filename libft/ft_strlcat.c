/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/14 20:20:57 by dopereir          #+#    #+#             */
/*   Updated: 2024/04/14 21:34:50 by dopereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dest, const char *src, size_t n)
{
	size_t	i;
	size_t	slen;
	size_t	dlen;

	slen = ft_strlen(src);
	dlen = ft_strlen(dest);
	i = 0;
	if (dlen >= n)
		return (slen + n);
	while (src[i] && dlen + i < n - 1)
	{
		dest[dlen + i] = src[i];
		i++;
	}
	if (n)
		dest[dlen + i] = '\0';
	return (dlen + slen);
}
