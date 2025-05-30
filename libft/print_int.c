/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_int.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/22 10:40:33 by dopereir          #+#    #+#             */
/*   Updated: 2024/08/18 01:32:17 by dopereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	print_int(t_flags *flags, t_list *op)
{
	char	*str;
	int		value;
	int		padding;
	int		sign;

	value = va_arg(op->ap, int);
	if (value < 0)
		str = ft_itoa(-value);
	else
		str = ft_itoa(value);
	if (!str)
		return ;
	sign = 0;
	if (value < 0 || flags->padding == '+' || flags->padding == ' ')
		sign = 1;
	padding = calculate_padding_bonus(value, flags, ft_strlen(str), sign);
	handle_padding_bonus(flags, padding, value, op);
	if (flags->precision > (int)ft_strlen(str) && value != INT_MIN)
		print_padding(flags->precision - ft_strlen(str), '0', op);
	if (!(flags->precision == 0 && value == 0))
		print_integer_str_bonus(str, op, flags);
	if (flags->left_align == '-' && padding > 0)
		print_padding(padding, ' ', op);
	free(str);
}
