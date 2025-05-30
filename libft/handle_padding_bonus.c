/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_padding_bonus.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/28 16:48:25 by dopereir          #+#    #+#             */
/*   Updated: 2024/08/18 02:00:18 by dopereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	edge_cases(t_flags *flags, int padding, int value, t_list *op)
{
	if (flags->zero_pad == 1 && flags->precision == 0 && value != 0)
	{
		print_padding(padding - 1, ' ', op);
		print_sign_bonus(flags, value, op);
	}
	else
	{
		print_padding(padding, ' ', op);
		print_sign_bonus(flags, value, op);
	}
}

void	handle_padding_bonus(t_flags *flags, int padding, size_t value,
	t_list *op)
{
	if (flags->zero_pad == 1 && flags->left_align != '-'
		&& flags->precision < 0)
	{
		print_sign_bonus(flags, value, op);
		print_padding(padding, '0', op);
	}
	else if (!flags->left_align)
	{
		if (value == 0 && flags->precision == 0 && flags->width > 0)
			print_padding(flags->width, ' ', op);
		else if (flags->width > 0 && padding > 0)
		{
			edge_cases(flags, padding, value, op);
		}
		else
			print_sign_bonus(flags, value, op);
	}
	else
		print_sign_bonus(flags, value, op);
}
