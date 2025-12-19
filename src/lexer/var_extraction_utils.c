/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_extraction_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsarraci <lsarraci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/19 18:18:46 by lsarraci          #+#    #+#             */
/*   Updated: 2025/12/19 18:33:38 by lsarraci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/shell.h"

char	*extract_name_variable(char *input, int *i)
{
	int		start;
	size_t	len;
	char	*var_name;

	if (!ft_isalpha(input[*i]) && input[*i] != '_')
		return (NULL);
	start = *i;
	while (input[*i] && is_var_name_char(input[*i]))
		(*i)++;
	len = *i - start;
	var_name = ft_substr(input, start, len);
	if (!var_name)
		return (NULL);
	return (var_name);
}

char	*extract_braced_variable(char *input, int *i)
{
	int		start;
	size_t	len;
	char	*var_name;

	if (input[*i] != '{')
		return (NULL);
	(*i)++;
	if (!ft_isalpha(input[*i]) && input[*i] != '_')
		return (NULL);
	start = *i;
	while (input[*i] && is_var_name_char(input[*i]) && input[*i] != '}')
		(*i)++;
	if (input[*i] != '}')
		return (NULL);
	len = *i - start;
	var_name = ft_substr(input, start, len);
	(*i)++;
	if (!var_name)
		return (NULL);
	return (var_name);
}

char	*extract_special_variable(char *input, int *i)
{
	char	*var_name;

	if (!is_special_var_char(input[*i]))
		return (NULL);
	var_name = ft_substr(input, *i, 1);
	(*i)++;
	if (!var_name)
		return (NULL);
	return (var_name);
}

char	*extract_positional_variable(char *input, int *i)
{
	int		start;
	size_t	len;
	char	*var_name;

	if (!ft_isdigit(input[*i]))
		return (NULL);
	start = *i;
	while (input[*i] && ft_isdigit(input[*i]))
		(*i)++;
	len = *i - start;
	var_name = ft_substr(input, start, len);
	if (!var_name)
		return (NULL);
	return (var_name);
}
