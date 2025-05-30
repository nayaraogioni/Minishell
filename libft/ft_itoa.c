/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/05 23:34:27 by dopereir          #+#    #+#             */
/*   Updated: 2024/05/25 17:39:07 by dopereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

static int	digit_count(int n)
{
	int	digit;

	digit = !n;
	while (n)
	{
		n /= 10;
		digit++;
	}
	return (digit);
}

char	*ft_itoa(int n)
{
	bool	sign;
	int		d_count;
	char	*result;

	sign = n < 0;
	d_count = digit_count(n) + sign;
	result = (char *)malloc(sizeof(char) * (d_count + 1));
	if (!result)
		return (NULL);
	result[d_count] = 0;
	if (sign)
	{
		*result = '-';
		result[--d_count] = -(n % 10) + '0';
		n = -(n / 10);
	}
	while (d_count-- - sign)
	{
		result[d_count] = n % 10 + '0';
		n /= 10;
	}
	return (result);
}

/* #include <stdio.h>

int	main(void)
{
	char	*str;

	str = ft_itoa(0);
	printf("%s\n", str);
	free(str);
	str = ft_itoa(123);
	printf("%s\n", str);
	free(str);
	str = ft_itoa(-123);
	printf("%s\n", str);
	free(str);
	str = ft_itoa(INT_MIN);
	printf("%s\n", str);
	free(str);
	return (0);
} */