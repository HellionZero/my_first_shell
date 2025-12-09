/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsarraci <lsarraci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 11:47:01 by lsarraci          #+#    #+#             */
/*   Updated: 2025/07/23 14:20:56 by lsarraci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	unsigned char		*str_dest;
	const unsigned char	*str_src;
	size_t				i;

	str_dest = (unsigned char *)dest;
	str_src = (unsigned char *)src;
	if (str_dest == str_src || n == 0)
		return (dest);
	if (str_dest < str_src)
	{
		i = 0;
		while (i < n)
		{
			str_dest[i] = str_src[i];
			i++;
		}
	}
	else
	{
		while (n--)
			str_dest[n] = str_src[n];
	}
	return (dest);
}
