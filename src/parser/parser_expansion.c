/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_expansion.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsarraci <lsarraci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/22 17:19:18 by lsarraci          #+#    #+#             */
/*   Updated: 2025/12/22 17:57:03 by lsarraci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/shell.h"

char	*expand_variable(char *var_name)
{
	char	*value;

	if (!var_name)
		return (ft_strdup(""));
	if (is_special_var(var_name))
		return (expand_special_var(var_name));
	value = getenv(var_name);
	if (!value)
		return (ft_strdup(""));
	return (ft_strdup(value));
}

char	*expand_word(t_word_part *parts)
{
	char		*result;
	char		*temp;
	char		*expanded;

	result = ft_strdup("");
	while (parts)
	{
		if (parts->type == PART_LITERAL || parts->type == PART_SINGLE_QUOTE
			|| parts->type == PART_DOUBLE_QUOTE)
			temp = ft_strjoin(result, parts->content);
		else if (parts->type == PART_VARIABLE)
		{
			expanded = expand_variable(parts->content);
			temp = ft_strjoin(result, expanded);
			free(expanded);
		}
		free(result);
		result = temp;
		parts = parts->next;
	}
	return (result);
}
