/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsarraci <lsarraci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 12:43:47 by lsarraci          #+#    #+#             */
/*   Updated: 2025/07/31 17:05:57 by lsarraci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr(char *str, int character)
{
	int	i;

	i = 0;
	while (str[i] != 0)
	{
		if (str[i] == (char)character)
			return (str + i);
		i++;
	}
	if (str[i] == (char)character)
		return (str + i);
	return (0);
}
