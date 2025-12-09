/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsarraci <lsarraci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 12:55:18 by lsarraci          #+#    #+#             */
/*   Updated: 2025/07/24 19:15:55 by lsarraci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *str, int c)
{
	const char		*lch;

	lch = NULL;
	while (*str != 0)
	{
		if (*str == (char)c)
			lch = str;
		str++;
	}
	if ((char)c == 0)
		lch = str;
	return ((char *)(lch));
}

/*if (!str)
		return (NULL); anti-crash*/
