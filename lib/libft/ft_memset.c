/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsarraci <lsarraci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 18:10:58 by lsarraci          #+#    #+#             */
/*   Updated: 2025/07/24 19:09:17 by lsarraci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset(void *str, int c, size_t n)
{
	unsigned char	*dest;
	size_t			i;

	i = 0;
	dest = (unsigned char *)str;
	while (i < n)
	{
		dest[i] = (unsigned char)c;
		i++;
	}
	return ((void *)dest);
}

/*if (!str)
		return (0); serve para não crashar quando 
		o programa insere um caractere nulo */