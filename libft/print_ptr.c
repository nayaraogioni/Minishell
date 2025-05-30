/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_ptr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/27 17:30:10 by dopereir          #+#    #+#             */
/*   Updated: 2024/08/08 16:58:58 by dopereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	ft_putstr(const char *str, t_list *op)
{
	while (*str)
	{
		ft_putchar(*str);
		str++;
		op->count++;
	}
}

static void	null_ptr_case(t_flags *flags, t_list *op)
{
	if (flags->width > 0 && flags->left_align != '-')
		print_padding(flags->width - 5, ' ', op);
	ft_putstr("(nil)", op);
}

void	print_ptr(t_flags *flags, t_list *op)
{
	void	*ptr;
	char	*str;
	int		len;

	ptr = va_arg(op->ap, void *);
	str = ft_itoa_custombase((unsigned long)ptr, 16);
	len = ft_strlen(str) + 2;
	if (flags->padding != ' ' && flags->width - len > 0 && ptr != NULL
		&& flags->left_align != '-')
		print_padding(flags->width - len, ' ', op);
	if (!ptr)
		null_ptr_case(flags, op);
	else
	{
		ft_putstr("0x", op);
		ft_putstr(str, op);
	}
	if (flags->left_align == '-' && flags->width - len > 0)
	{
		if (!ptr)
			print_padding(flags->width - 5, ' ', op);
		else
			print_padding(flags->width - len, ' ', op);
	}
	free(str);
}

/*int main()
{
    int x = 42;
    int *ptr = &x;

    // Test cases
    ft_printf("Pointer value: %p\n", ptr);
    ft_printf("Pointer value with width: %20p\n", ptr);
    ft_printf("Pointer value with left align: %-20p\n", ptr);
    ft_printf("Pointer value with precision: %.8p\n", ptr);

    return 0;
}*/
