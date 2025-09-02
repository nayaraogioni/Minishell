/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtol.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 15:05:37 by dopereir          #+#    #+#             */
/*   Updated: 2025/09/02 16:31:48 by dopereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>
#include <limits.h>
#include <ctype.h>
#include <stdio.h>

//skips leading blank spaces and handle sign, in this order.
void	prep_strtol(const char **ptr, int *sign)
{
	while (ft_isspace((unsigned char)**ptr))
		(*ptr)++;
	if (**ptr == '+' || **ptr == '-')
	{
		if (**ptr == '-')
			*sign = -1;
		(*ptr)++;
	}
}

//handles base prefix (0x or 0X), and validates base range (2-36)
int	handle_base(const char **ptr, int *base, char **endptr, const char *nptr)
{
	if (*base == 0)
	{
		if (**ptr == '0')
		{
			if ((*ptr)[1] == 'x' || (*ptr)[1] == 'X')
			{
				*base = 16;
				*ptr += 2;
			}
			else
				*base = 8;
		}
		else
			*base = 10;
	}
	if (*base < 2 || *base > 36)
	{
		if (endptr)
			*endptr = (char *)nptr;
		return (0);
	}
	return (1);
}

int	get_digit(char c)
{
	int	value;

	value = -1;
	if (ft_isdigit((unsigned char)c))
		value = c - '0';
	else if (ft_isalpha((unsigned char)c))
		value = ft_tolower(c) - 'a' + 10;
	return (value);
}

int	handle_digits(const char **ptr, int *base, long *result, int sign)
{
	int			d;
	int			found;
	long int	overflow;

	found = 0;
	overflow = 0;
	while (*ptr && **ptr)
	{
		d = get_digit(**ptr);
		if (d < 0 || d >= *base)
			break ;
		overflow = check_overflow(result, d, *base, sign);
		if (overflow != 0)
		{
			*result = overflow;
			break ;
		}
		*result = *result * *base + d;
		(*ptr)++;
		found = 1;
	}
	return (found);
}

long	ft_strtol(const char *nptr, char **endptr, int base)
{
	const char	*ptr;
	long		result;
	int			sign;
	int			found;

	ptr = nptr;
	result = 0;
	sign = 1;
	prep_strtol(&ptr, &sign);
	if (!handle_base(&ptr, &base, endptr, nptr))
		return (0);
	found = handle_digits(&ptr, &base, &result, sign);
	if (endptr)
	{
		if (found)
			*endptr = (char *)ptr;
		else
			*endptr = (char *)nptr;
	}
	return (sign * result);
}
