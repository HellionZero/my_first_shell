/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_converter.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsarraci <lsarraci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 16:27:29 by lsarraci          #+#    #+#             */
/*   Updated: 2025/08/09 19:49:15 by lsarraci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_converter(const char ch, va_list lst)
{
	if (ch == '%')
		return (ft_putchar('%'));
	else if (ch == 'c')
		return (ft_putchar(va_arg(lst, int)));
	else if (ch == 's')
		return (ft_putstr(va_arg(lst, char *)));
	else if (ch == 'p')
		return (ft_printptr(va_arg(lst, unsigned long)));
	else if (ch == 'd' || ch == 'i')
		return (ft_printnbr(va_arg(lst, int)));
	else if (ch == 'u')
		return (ft_printmod(va_arg(lst, int)));
	else if (ch == 'x')
		return (ft_printhex(va_arg(lst, unsigned int), 0));
	else if (ch == 'X')
		return (ft_printhex(va_arg(lst, unsigned int), 1));
	return (0);
}
