/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   general_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsarraci <lsarraci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/23 17:34:20 by lsarraci          #+#    #+#             */
/*   Updated: 2025/12/23 17:34:50 by lsarraci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/shell.h"

char	*join_and_free(char *s1, char *s2)
{
	char	*result;

	if (!s1 || !s2)
	{
		if (s1)
			return (s1);
		if (s2)
			return (ft_strdup(s2));
		return (ft_strdup(""));
	}
	result = ft_strjoin(s1, s2);
	free(s1);
	return (result);
}

char	*join_char_and_free(char *s, char c)
{
	char	*temp;
	char	char_str[2];

	char_str[0] = c;
	char_str[1] = '\0';
	temp = ft_strjoin(s, char_str);
	free(s);
	return (temp);
}

int	has_quotes(char c)
{
	return (c == '\'' || c == '"');
}

int	is_var_char(char c)
{
	return (ft_isalnum(c) || c == '_');
}

int	needs_expansion(char *str)
{
	if (!str)
		return (0);
	return (ft_strchr(str, '$') != NULL);
}
