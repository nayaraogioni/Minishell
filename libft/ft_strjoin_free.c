/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin_free.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 20:31:56 by dopereir          #+#    #+#             */
/*   Updated: 2025/08/01 20:44:27 by dopereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

//NOTE: If s2 is a literal string like "42porto!" use only flag 'L'.
// 'A' flag is to free both strings, 'L'(s1) is for left, 'R'(s2) is for right
char	*ft_strjoin_free(char const *s1, char const *s2, char flag)
{
	size_t	slen1;
	size_t	slen2;
	char	*s3;

	slen1 = 0;
	slen2 = 0;
	if (s1)
		slen1 = ft_strlen(s1);
	if (s2)
		slen2 = ft_strlen(s2);
	s3 = (char *)malloc((slen1 + slen2 + 1) * sizeof(char));
	if (!s3)
		return (NULL);
	s3[0] = '\0';
	if (s1)
		ft_strlcpy(s3, s1, slen1 + 1);
	if (s2)
		ft_strlcat(s3, s2, slen1 + slen2 + 1);
	if ((flag == 'L' || flag == 'A') && s1)
		free ((void *)s1);
	if ((flag == 'R' || flag == 'A') && s2)
		free ((void *)s2);
	return (s3);
}
