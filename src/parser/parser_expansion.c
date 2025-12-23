/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_expansion.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsarraci <lsarraci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/22 17:19:18 by lsarraci          #+#    #+#             */
/*   Updated: 2025/12/23 17:21:43 by lsarraci         ###   ########.fr       */
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
	char		*expanded;

	result = ft_strdup("");
	while (parts)
	{
		if (parts->type == PART_LITERAL || parts->type == PART_SINGLE_QUOTE
			|| parts->type == PART_DOUBLE_QUOTE)
			result = join_and_free(result, parts->content);
		else if (parts->type == PART_VARIABLE)
		{
			expanded = expand_variable(parts->content);
			result = join_and_free(result, expanded);
			free(expanded);
		}
		parts = parts->next;
	}
	return (result);
}
