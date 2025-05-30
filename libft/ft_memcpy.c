/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/13 18:23:43 by dopereir          #+#    #+#             */
/*   Updated: 2024/05/25 17:10:50 by dopereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	unsigned char	*tempsrc;
	unsigned char	*tempdest;

	tempsrc = (unsigned char *)src;
	tempdest = (unsigned char *)dest;
	if (!dest && !src)
		return (NULL);
	while (n--)
	{
		*tempdest++ = *tempsrc++;
	}
	return (dest);
}
/* unsigned char is better to handle raw bytes and binary data,
* that means is better to use on functions that manipulate memory blocks
* directly.*/