/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strspn.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 21:41:22 by dopereir          #+#    #+#             */
/*   Updated: 2025/06/04 21:51:52 by dopereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strspn(char *s1, char *s2)
{
	char	*sc1;
	char	*sc2;

	sc1 = s1;
	while (*sc1 != '\0')
	{
		sc2 = s2;
		while (1)
		{
			if (*sc2 == '\0')
				return (sc1 - s1);
			else if (*sc1 == *sc2)
				break ;
			++sc2;
		}
		++sc1;
	}
	return (sc1 - s1);
}
