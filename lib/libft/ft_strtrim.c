/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsarraci <lsarraci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 12:48:11 by lsarraci          #+#    #+#             */
/*   Updated: 2025/07/30 18:46:23 by lsarraci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_trimmer(char *ch, const char c)
{
	int	i;

	i = 0;
	while (ch[i] != 0)
	{
		if (ch[i] == c)
			return (1);
		i++;
	}
	return (0);
}

static char	*final_char(const char *s1, size_t start, size_t len)
{
	size_t	i;
	char	*s2;

	i = 0;
	if (start >= ft_strlen(s1))
		return (ft_strdup(""));
	s2 = ft_calloc(len + 1, sizeof(char));
	if (!s2)
		return (NULL);
	while (i < len)
	{
		s2[i] = s1[start + i];
		i++;
	}
	return (s2);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	size_t	i;
	size_t	j;

	i = 0;
	j = ft_strlen(s1) - 1;
	if (ft_strlen(s1) == 0)
		return (ft_strdup(""));
	while (ft_trimmer((char *)set, s1[i]))
		i++;
	while (ft_trimmer((char *)set, s1[j]))
		j--;
	return (final_char(s1, i, j - (i - 1)));
}
