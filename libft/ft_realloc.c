/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_realloc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 09:22:48 by dopereir          #+#    #+#             */
/*   Updated: 2024/10/25 09:25:35 by dopereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_realloc(void *ptr, size_t oldsize, size_t size)
{
	void	*new;
	size_t	minsize;

	if (size == 0)
	{
		free(ptr);
		return (NULL);
	}
	if (ptr == NULL)
		return (malloc(size));
	new = malloc(size);
	if (!new)
		return (NULL);
	minsize = size;
	if (oldsize < size)
		minsize = oldsize;
	ft_memcpy(new, ptr, minsize);
	while (size - oldsize != 0)
		((char *)new)[oldsize++] = '\0';
	free(ptr);
	return (new);
}
