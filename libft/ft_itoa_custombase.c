/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa_custombase.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/29 16:20:32 by dopereir          #+#    #+#             */
/*   Updated: 2024/08/06 18:36:45 by dopereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_itoa_custombase(unsigned long value, int base)
{
	char			*str;
	char			*digits;
	int				len;
	unsigned long	tmp;

	digits = "0123456789abcdef";
	len = 0;
	tmp = value;
	while (tmp != 0)
	{
		tmp /= base;
		len++;
	}
	if (value == 0)
		len = 1;
	str = (char *)malloc(len + 1);
	if (!str)
		return (str);
	str[len] = '\0';
	while (len-- > 0)
	{
		str[len] = digits[value % base];
		value /= base;
	}
	return (str);
}
