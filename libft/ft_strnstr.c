/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/25 15:55:58 by dopereir          #+#    #+#             */
/*   Updated: 2024/05/06 09:32:00 by dopereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *hay, const char *needle, size_t len)
{
	size_t	needle_len;

	if (*needle == '\0')
		return ((char *)hay);
	needle_len = ft_strlen(needle);
	while (*hay != '\0' && len-- >= needle_len)
	{
		if (*hay == *needle && ft_strncmp(hay, needle, needle_len) == 0)
			return ((char *)hay);
		hay++;
	}
	return (NULL);
}
