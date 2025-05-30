/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_char.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/25 20:50:59 by dopereir          #+#    #+#             */
/*   Updated: 2024/08/17 19:02:10 by dopereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	print_char(t_flags *flags, t_list *op)
{
	char	c;

	c = va_arg(op->ap, int);
	if (flags->width > 1 && flags->left_align != '-')
		print_padding(flags->width - 1, ' ', op);
	ft_putchar(c);
	op->count++;
	if (flags->width > 1 && flags->left_align == '-')
		print_padding(flags->width - 1, ' ', op);
}
