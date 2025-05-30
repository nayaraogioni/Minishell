/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_string.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/25 20:51:17 by dopereir          #+#    #+#             */
/*   Updated: 2024/08/14 00:38:42 by dopereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	adjust_len(t_flags *flags, int len, int was_null)
{
	if (was_null)
	{
		if (flags->precision >= 0 && flags->precision < 6)
			return (0);
		if (flags->precision >= 6)
			return (6);
	}
	else if (flags->precision >= 0 && flags->precision < len)
		return (flags->precision);
	return (len);
}

static void	print_str(char *str, int len, t_list *op)
{
	while (len--)
	{
		ft_putchar(*str++);
		op->count++;
	}
}

void	print_string(t_flags *flags, t_list *op)
{
	char	*str;
	int		was_null;
	int		len;
	int		padding;

	str = va_arg(op->ap, char *);
	was_null = 0;
	if (!str)
	{
		str = "(null)";
		was_null = 1;
	}
	len = adjust_len(flags, ft_strlen(str), was_null);
	padding = 0;
	if (flags->width > len)
		padding = flags->width - len;
	if (flags->left_align != '-')
		print_padding(padding, ' ', op);
	print_str(str, len, op);
	if (flags->left_align == '-')
		print_padding(padding, ' ', op);
}

/* OLD VERSION BELOW, SUBJECT TO STUDY
static int	was_null_helper(t_flags *flags, int len)
{
	if (flags->precision >= 0 && flags->precision < 6)
		len = 0;
	else if (flags->precision >= 6)
		len = 6;
	return (len);
}

void	print_string(t_flags *flags, t_list *op)
{
	char	*str;
	int		len;
	int		padding;
	int		i;
	int		was_null;

	str = va_arg(op->ap, char *);
	was_null = 0;
	if (!str)
	{
		str = "(null)";
		was_null = 1;
	}
	len = ft_strlen(str);
	if (was_null)
		len = was_null_helper(flags, len);
	else if (flags->precision >= 0 && flags->precision < len)
		len = flags->precision;
	padding = 0;
	if (flags->width > len)
		padding = flags->width - len;
	if (flags->left_align != '-')
		print_padding(padding, ' ', op);
	i = 0;
	while (i < len)
	{
		ft_putchar(str[i]);
		i++;
		op->count++;
	}
	if (flags->left_align == '-')
		print_padding(padding, ' ', op);
}
*/
