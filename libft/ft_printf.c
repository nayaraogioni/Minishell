/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/25 20:50:25 by dopereir          #+#    #+#             */
/*   Updated: 2024/07/18 22:48:36 by rache            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	handle_specifier(t_flags *flags, t_list *op)
{
	if (flags->specifier == 'd' || flags->specifier == 'i')
		print_int(flags, op);
	else if (flags->specifier == 'c')
		print_char(flags, op);
	else if (flags->specifier == 's')
		print_string(flags, op);
	else if (flags->specifier == 'p')
		print_ptr(flags, op);
	else if (flags->specifier == 'x' || flags->specifier == 'X')
		print_hex(flags, op);
	else if (flags->specifier == 'u')
		print_unsigned(flags, op);
}

static void	print_char_local(char c, t_list *op)
{
	ft_putchar(c);
	op->count++;
}

static void	parse_and_print(const char *format, t_list *op)
{
	size_t	start_i;
	t_flags	flags;

	flags.width = 0;
	flags.precision = -1;
	flags.specifier = 0;
	flags.padding = 0;
	flags.zero_pad = 0;
	flags.left_align = 0;
	start_i = op->i;
	parse_flags(&flags, format, &op->i);
	handle_specifier(&flags, op);
	op->i = op->i - start_i;
}

int	ft_printf(const char *format, ...)
{
	t_list	op;

	op.count = 0;
	va_start(op.ap, format);
	while (*format)
	{
		if (*format == '%')
		{
			format++;
			if (*format == '%')
				print_char_local('%', &op);
			else
			{
				op.i = 0;
				parse_and_print(format, &op);
				format += op.i;
				continue ;
			}
		}
		else
			print_char_local(*format, &op);
		format++;
	}
	va_end(op.ap);
	return (op.count);
}
