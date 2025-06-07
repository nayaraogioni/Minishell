/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcspn.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 21:35:08 by dopereir          #+#    #+#             */
/*   Updated: 2025/06/04 21:51:34 by dopereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strcspn(char *s1, char *s2)
{
	char	*sc1;
	char	*sc2;

	sc1 = s1;
	while (*sc1 != '\0')
	{
		sc2 = s2;
		while (*sc2 != '\0')
		{
			if (*sc1 == *sc2)
				return (sc1 - s1);
			++sc2;
		}
		++sc1;
	}
	return (sc1 - s1);
}
