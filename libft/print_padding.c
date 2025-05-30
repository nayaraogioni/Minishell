/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_padding.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dopereir <dopereir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/25 20:51:09 by dopereir          #+#    #+#             */
/*   Updated: 2024/08/08 16:31:15 by dopereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	print_padding(int length, char pad_char, t_list *op)
{
	int	i;

	i = 0;
	while (i < length)
	{
		ft_putchar(pad_char);
		i++;
		op->count++;
	}
}
