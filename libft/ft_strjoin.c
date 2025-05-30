/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/25 16:30:24 by dopereir          #+#    #+#             */
/*   Updated: 2024/04/25 17:08:17 by dopereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

char	*ft_strjoin(char const *s1, char const *s2)
{
	size_t	slen1;
	size_t	slen2;
	char	*s3;

	slen1 = ft_strlen(s1);
	slen2 = ft_strlen(s2);
	s3 = (char *)malloc((slen1 + slen2 + 1) * sizeof(char));
	if (s3 == NULL)
		return (NULL);
	ft_strlcpy(s3, s1, slen1 + 1);
	ft_strlcat(s3, s2, slen1 + slen2 + 1);
	return (s3);
}
