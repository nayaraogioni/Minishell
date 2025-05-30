/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/13 15:26:19 by dopereir          #+#    #+#             */
/*   Updated: 2024/05/06 00:18:59 by dopereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <unistd.h>

void	*ft_memset(void *str, int c, size_t n)
//(string we want to memset, value we want to allocate in the string, 
// number of times we gonna allocate) 
{
	unsigned char	*p;

	p = (unsigned char *)str;
	while (n--)
	{
		*p++ = (unsigned char)c;
	}
	return (str);
}
/*While the 'n' isn´t 0 we gonna itinerate through "*p" variable. 
Meanwhile, "*p" is equal to 'x'. Three glasses of wine*/
