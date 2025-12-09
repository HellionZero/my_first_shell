/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_realloc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsarraci <lsarraci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 16:09:22 by lsarraci          #+#    #+#             */
/*   Updated: 2025/11/03 15:38:27 by lsarraci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_realloc(void *ptr, size_t size)
{
	void	*rsz_ptr;

	if (!ptr)
		return (malloc(size));
	if (!size)
		return (ptr);
	rsz_ptr = malloc(size);
	ft_memcpy(rsz_ptr, ptr, size);
	return (rsz_ptr);
}
