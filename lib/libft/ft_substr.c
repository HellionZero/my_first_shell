/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsarraci <lsarraci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 19:59:45 by lsarraci          #+#    #+#             */
/*   Updated: 2025/07/25 18:22:36 by lsarraci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char		*sub;
	size_t		size;
	size_t		sublen;
	size_t		i;

	if (!s)
		return (NULL);
	i = 0;
	size = ft_strlen(s);
	if (start >= size)
		sublen = 0;
	else if (len > size - start)
		sublen = size - start;
	else
		sublen = len;
	sub = malloc(sublen + 1);
	if (!sub)
		return (NULL);
	while (sublen > i)
	{
		sub[i] = s[start + i];
		i++;
	}
	sub[i] = '\0';
	return (sub);
}
