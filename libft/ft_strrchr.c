/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 18:07:35 by dopereir          #+#    #+#             */
/*   Updated: 2024/05/18 19:26:02 by dopereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdio.h>

char	*ft_strrchr(const char *s, int c)
{
	const char	*last_oc;

	last_oc = NULL;
	while (1)
	{
		if (*s == (char)c)
		{
			last_oc = s;
		}
		if (*s == '\0')
		{
			break ;
		}
		s++;
	}
	return ((char *)last_oc);
}
/*int   main()
{
        const char      test[] = "Rache bartmoss, rabids creator";
        char    *c;

        c = ft_strrchr(test, 's');

        if (c != NULL)
                printf("Character found at position %ld\n", c - test + 1);
        else
                printf("Character not found in the string.\n");
        return (0);
}*/
