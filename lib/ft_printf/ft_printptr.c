/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printptr.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsarraci <lsarraci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 19:41:52 by lsarraci          #+#    #+#             */
/*   Updated: 2025/08/09 19:44:53 by lsarraci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static int	ft_set_ptr(unsigned long ch)
{
	char	*base;
	char	c;
	int		i;

	i = 0;
	base = "0123456789abcdef";
	if (ch >= 16)
		i += ft_set_ptr(ch / 16);
	c = base[ch % 16];
	i += ft_putchar(c);
	return (i);
}

int	ft_printptr(unsigned long ptr)
{
	int		i;

	i = 0;
	if (!ptr)
		return (ft_putstr("(nil)"));
	i += ft_putstr("0x");
	i += ft_set_ptr(ptr);
	return (i);
}
