/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_prefix_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/28 16:56:15 by dopereir          #+#    #+#             */
/*   Updated: 2024/07/28 16:59:10 by dopereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	print_prefix_bonus(int prefix_len, t_flags *flags, t_list *op)
{
	if (flags->padding == '#' && prefix_len > 0)
	{
		if (flags->specifier == 'X')
		{
			ft_putchar('0');
			ft_putchar('X');
			op->count += 2;
			return (2);
		}
		else if (flags->specifier == 'x')
		{
			ft_putchar('0');
			ft_putchar('x');
			op->count += 2;
			return (2);
		}
	}
	return (0);
}
