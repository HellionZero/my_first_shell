/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printmod.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsarraci <lsarraci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 17:03:30 by lsarraci          #+#    #+#             */
/*   Updated: 2025/08/09 16:03:10 by lsarraci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_printmod(unsigned int n)
{
	int	i;

	i = 0;
	if (n < 10)
		i += ft_putchar(n + '0');
	else
	{
		i += ft_printnbr(n / 10);
		i += ft_printnbr(n % 10);
	}
	return (i);
}
