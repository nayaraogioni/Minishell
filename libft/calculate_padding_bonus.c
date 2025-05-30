/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   calculate_padding_bonus.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/28 16:38:50 by dopereir          #+#    #+#             */
/*   Updated: 2024/08/18 01:33:16 by dopereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	calculate_padding_bonus(int value, t_flags *flags, int len, int sign)
{
	int	total_len;

	total_len = len;
	if (flags->precision > len)
		total_len = flags->precision;
	if (sign || flags->padding == ' ')
		total_len++;
	if (flags->width >= total_len)
	{
		if (flags->precision == 0 || value == INT_MIN)
			return (flags->width - total_len + 1);
		else
			return (flags->width - total_len);
	}
	else
		return (0);
}
