/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_variables.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsarraci <lsarraci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/19 14:40:04 by lsarraci          #+#    #+#             */
/*   Updated: 2025/12/19 18:41:31 by lsarraci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/shell.h"

static char	*get_variable_name(char *input, int *i)
{
	char	*var_name;

	if (input[*i] == '{')
		var_name = extract_braced_variable(input, i);
	else if (is_special_var_char(input[*i]))
		var_name = extract_special_variable(input, i);
	else if (ft_isdigit(input[*i]))
		var_name = extract_positional_variable(input, i);
	else
		var_name = extract_name_variable(input, i);
	return (var_name);
}

t_word_part	*extract_variable(char *input, int *i)
{
	t_word_part	*part;
	char		*var_name;

	if (input[*i] != '$')
		return (NULL);
	(*i)++;
	if (input[*i] == '\0')
	{
		(*i)--;
		return (NULL);
	}
	var_name = get_variable_name(input, i);
	if (!var_name)
	{
		(*i)--;
		return (NULL);
	}
	part = word_part_new(PART_VARIABLE, var_name);
	if (!part)
	{
		free(var_name);
		return (NULL);
	}
	return (part);
}
