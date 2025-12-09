/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsarraci <lsarraci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 13:42:52 by lsarraci          #+#    #+#             */
/*   Updated: 2025/08/14 14:50:11 by lsarraci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_printf(const char *fstring, ...)
{
	va_list	ls;
	int		count;
	int		i;

	i = 0;
	count = 0;
	va_start(ls, fstring);
	while (fstring[i] != '\0')
	{
		if (fstring[i] == '%')
		{
			count += ft_converter(fstring[i + 1], ls);
			i++;
		}
		else
			count += ft_putchar(fstring[i]);
		i++;
	}
	va_end(ls);
	return (count);
}
