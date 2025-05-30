/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_hex.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/27 17:29:52 by dopereir          #+#    #+#             */
/*   Updated: 2024/08/17 18:54:14 by dopereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	ft_putstr(const char *str, t_list *op, t_flags *flags)
{
	while (*str)
	{
		if (flags->specifier == 'X' && *str >= 'a' && *str <= 'z')
			ft_putchar(*str - 32);
		else
		{
			if (flags->specifier == 'x' && *str >= 'A' && *str <= 'Z')
				ft_putchar(*str + 32);
			else
				ft_putchar(*str);
		}
		str++;
		op->count++;
	}
}

static int	prefix_len_func(t_flags *flags, int value)
{
	int	prefix_len;

	prefix_len = 0;
	if (flags->padding == '#' && value != 0)
		prefix_len = 2;
	return (prefix_len);
}

static int	handle_flags(t_flags *flags, unsigned int value, int len,
	t_list *op)
{
	int		prefix_len;
	int		padding;
	char	*str_value;

	str_value = ft_itoa_custombase(value, 16);
	prefix_len = prefix_len_func(flags, value);
	if (flags->precision > len)
		len = flags->precision;
	padding = 0;
	if (flags->width >= len + prefix_len && value == 0 && flags->precision == 0)
		print_padding(flags->width - (len + prefix_len) + 1, ' ', op);
	else if (flags->width > len + prefix_len)
		padding = flags->width - (len + prefix_len);
	if (flags->left_align != '-')
	{
		if (flags->zero_pad == 1 && flags->precision < 0)
			print_padding(padding, '0', op);
		else
			print_padding(padding, ' ', op);
	}
	print_prefix_bonus(prefix_len, flags, op);
	if (flags->precision > (int)ft_strlen(str_value))
		print_padding(flags->precision - ft_strlen(str_value), '0', op);
	free(str_value);
	return (padding);
}

void	print_hex(t_flags *flags, t_list *op)
{
	char			*str;
	unsigned int	value;
	int				len;
	int				padding;

	value = va_arg(op->ap, unsigned int);
	str = ft_itoa_custombase(value, 16);
	len = ft_strlen(str);
	padding = handle_flags(flags, value, len, op);
	if (flags->precision != 0 || value != 0)
		ft_putstr(str, op, flags);
	if (flags->left_align == '-' && padding > 0)
		print_padding(padding, ' ', op);
	free(str);
}
