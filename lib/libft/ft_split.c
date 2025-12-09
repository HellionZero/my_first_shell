/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsarraci <lsarraci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 20:04:38 by lsarraci          #+#    #+#             */
/*   Updated: 2025/07/29 18:34:59 by lsarraci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_count_words(const char *str, char ch)
{
	int		i;
	int		point;
	short	check;

	i = 0;
	point = 0;
	check = 0;
	if (!str)
		return (0);
	while (str[i] != 0)
	{
		if (str[i] != ch && check == 0)
		{
			check = 1;
			point++;
		}
		else if (str[i] == ch)
			check = 0;
		i++;
	}
	return (point);
}

static void	ft_free(char **ch, int num)
{
	int	i;

	i = 0;
	while (i < num)
	{
		free(ch[i]);
		i++;
	}
	free(ch);
}

static char	*ft_set_word(char *str, size_t n)
{
	char		*word;
	size_t		i;

	i = 0;
	word = ft_calloc((n + 1), sizeof(char));
	if (!word)
		return (NULL);
	while (str[i] && i < n)
	{
		word[i] = str[i];
		i++;
	}
	return (word);
}

char	**ft_split(char const *s, char c)
{
	char	**words;
	char	*start;
	size_t	j;
	size_t	i;

	i = 0;
	j = ft_count_words(s, c);
	words = ft_calloc(j + 1, sizeof(char *));
	if (!words)
		return (NULL);
	while (i < j)
	{
		while (*s == c && *s != 0)
			s++;
		start = (char *)s;
		while (*s != c && *s != 0)
			s++;
		words[i] = ft_set_word(start, (char *)s - start);
		if (!words[i])
			ft_free(words, j);
		i++;
	}
	return (words);
}
