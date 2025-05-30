/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_sign_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/28 16:23:54 by dopereir          #+#    #+#             */
/*   Updated: 2024/08/17 19:27:08 by dopereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	print_sign_bonus(t_flags *flags, int value, t_list *op)
{
	if (value == INT_MIN && flags->zero_pad != 1)
		return ;
	else if (value == INT_MIN && flags->zero_pad == 1)
	{
		ft_putchar('-');
		op->count++;
	}
	else if (value < 0 && value != INT_MIN)
	{
		ft_putchar('-');
		op->count++;
	}
	else if (flags->padding == '+')
	{
		ft_putchar('+');
		op->count++;
	}
	else if (flags->padding == ' ')
	{
		ft_putchar(' ');
		op->count++;
	}
	else
		return ;
}
