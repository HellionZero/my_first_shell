/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printhex.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsarraci <lsarraci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 19:21:26 by lsarraci          #+#    #+#             */
/*   Updated: 2025/08/13 16:09:43 by lsarraci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_printhex(unsigned int num, int upper)
{
	char	*base;
	char	c;
	int		i;

	i = 0;
	if (upper == 1)
		base = "0123456789ABCDEF";
	else
		base = "0123456789abcdef";
	if (num >= 16)
		i += ft_printhex(num / 16, upper);
	c = base[num % 16];
	i += ft_putchar(c);
	return (i);
}
