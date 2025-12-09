/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsarraci <lsarraci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 19:57:49 by lsarraci          #+#    #+#             */
/*   Updated: 2025/07/24 18:53:48 by lsarraci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *s1, const void *s2, size_t n)
{
	size_t				i;
	unsigned char		*dest;
	const unsigned char	*src;

	dest = (unsigned char *)s1;
	src = (const unsigned char *)s2;
	i = 0;
	if (!s1 && !s2)
		return (0);
	while (i < n)
	{
		dest[i] = src[i];
		i++;
	}
	s1 = (void *)dest;
	return (s1);
}
/*#include<stdio.h>

int	main(void)
{
	const char 	src[20] = "Hello";
    char 		dst[20] = {1};

	ft_memcpy(dst, src, 2);
	printf("dst = %s\n", dst);
	return (0);
}*/
