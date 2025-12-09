/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsarraci <lsarraci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 18:07:14 by lsarraci          #+#    #+#             */
/*   Updated: 2025/07/31 17:04:08 by lsarraci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t nitems, size_t size)
{
	unsigned char	*p;
	size_t			bytesize;

	if (!size || !nitems)
		return (malloc(1));
	if ((size_t) - 1 / size < nitems)
		return (NULL);
	bytesize = nitems * size;
	p = (void *)malloc(bytesize);
	if (!p)
		return (NULL);
	ft_memset(p, 0, bytesize);
	return (p);
}
