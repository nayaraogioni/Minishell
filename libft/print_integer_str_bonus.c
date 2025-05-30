/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_integer_str_bonus.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/28 16:30:35 by dopereir          #+#    #+#             */
/*   Updated: 2024/08/18 01:30:16 by dopereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	negative_case(t_list *op, const char *str, t_flags *flags)
{
	int	j;

	if (flags->zero_pad != 1)
	{
		ft_putchar('-');
		op->count++;
	}
	if (flags->precision >= (int)ft_strlen(str))
		print_padding(flags->precision - ft_strlen(str) + 1, '0', op);
	j = 1;
	while (str[j])
	{
		ft_putchar(str[j++]);
		op->count++;
	}
}

void	print_integer_str_bonus(const char *str, t_list *op, t_flags *flags)
{
	int	i;
	int	value;

	value = ft_atoi(str);
	i = 0;
	if (value < 0)
		i = 1;
	if (0 > value)
		negative_case(op, str, flags);
	else
	{
		while (str[i] != '\0')
		{
			ft_putchar(str[i++]);
			op->count++;
		}
	}
}
