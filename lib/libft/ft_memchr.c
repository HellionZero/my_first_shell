/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsarraci <lsarraci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 13:19:41 by lsarraci          #+#    #+#             */
/*   Updated: 2025/07/24 19:16:34 by lsarraci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

//int	ft_strlen(char *ch);

void	*ft_memchr(const void *pointer, int value, size_t count)
{
	size_t			i;
	unsigned char	*charp;

	i = 0;
	charp = (unsigned char *)pointer;
	while (i < count)
	{
		if (charp[i] == (unsigned char)value)
			return ((void *)(charp + i));
		i++;
	}
	return (0);
}

/*int	main(void)
{
	char	*str;
	char	ch;
	char	*ret;

	ch = '.';
	str = "super.com";
	ret = ft_memchr(str, ch, ft_strlen(str));
	printf("String after |%c| is - |%s|\n", ch, ret);
	return (0);

	if (!pointer)
		return (0); anti-crash
}*/
