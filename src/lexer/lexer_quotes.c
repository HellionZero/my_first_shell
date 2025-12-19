/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_quotes.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsarraci <lsarraci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/19 14:29:03 by lsarraci          #+#    #+#             */
/*   Updated: 2025/12/19 17:56:56 by lsarraci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/shell.h"

static char	*extract_quote_content(char *input, int *i, char quote_char)
{
	char		*content;
	int			start;
	size_t		len;

	if (input[*i] != quote_char)
		return (NULL);
	(*i)++;
	start = *i;
	while (input[*i] && input[*i] != quote_char)
	{
		if (quote_char == '"' && input[*i] == '\\' && input[*i + 1])
			(*i)++;
		(*i)++;
	}
	if (input[*i] != quote_char)
		return (NULL);
	len = *i - start;
	content = ft_substr(input, start, len);
	if (!content)
		return (NULL);
	(*i)++;
	return (content);
}

static	t_word_part	*create_quote_part(char *input, int *i,
										char quote_char, t_word_part_type type)
{
	char		*content;
	t_word_part	*part;

	content = extract_quote_content(input, i, quote_char);
	if (!content)
		return (NULL);
	part = word_part_new(type, content);
	if (!part)
	{
		free(content);
		return (NULL);
	}
	return (part);
}

t_word_part	*extract_single_quoted(char *input, int *i)
{
	return (create_quote_part(input, i, '\'', PART_SINGLE_QUOTE));
}

t_word_part	*extract_double_quoted(char *input, int *i)
{
	return (create_quote_part(input, i, '"', PART_DOUBLE_QUOTE));
}
