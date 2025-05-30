/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/25 16:25:36 by dopereir          #+#    #+#             */
/*   Updated: 2024/04/25 16:26:14 by dopereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

char	*ft_strdup(const char *s)
{
	size_t	len;
	char	*duplicate;

	len = ft_strlen(s);
	duplicate = (char *)malloc((len + 1) * sizeof(char));
	if (duplicate != NULL)
		ft_strlcpy(duplicate, s, len + 1);
	return (duplicate);
}
