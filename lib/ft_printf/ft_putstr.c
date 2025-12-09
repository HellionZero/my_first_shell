/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putstr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsarraci <lsarraci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 14:27:22 by lsarraci          #+#    #+#             */
/*   Updated: 2025/08/09 15:13:56 by lsarraci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

long	ft_putstr(const char *ch)
{
	long	i;

	i = 0;
	if (ch == 0)
	{
		ft_putstr("(null)");
		return (6);
	}
	while (ch[i] != 0)
		ft_putchar(ch[i++]);
	return (i);
}
