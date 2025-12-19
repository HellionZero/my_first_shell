/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_words.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsarraci <lsarraci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/17 18:56:21 by lsarraci          #+#    #+#             */
/*   Updated: 2025/12/19 18:02:29 by lsarraci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/shell.h"

static t_word_part	*extract_literal(char *input, int *i)
{
	t_word_part		*token;
	char			*content;
	size_t			len;
	int				start;

	start = *i;
	while (input[*i] && is_not_special(input[*i])
		&& !is_word_boundary(input[*i]))
		(*i)++;
	len = *i - start;
	if (len == 0)
		return (NULL);
	content = ft_substr(input, start, len);
	if (!content)
		return (NULL);
	token = word_part_new(PART_LITERAL, content);
	if (!token)
	{
		free(content);
		return (NULL);
	}
	return (token);
}

static int	add_word_part(t_word_part **parts, t_word_part *new_part)
{
	if (!new_part)
	{
		word_part_list_free(*parts);
		return (0);
	}
	word_part_add_back(parts, new_part);
	return (1);
}

static t_word_part	*handle_dollar(char *input, int *i)
{
	t_word_part	*part;
	char		*dollar_str;

	part = extract_variable(input, i);
	if (!part)
	{
		dollar_str = ft_substr(input, *i, 1);
		part = word_part_new(PART_LITERAL, dollar_str);
		if (!part && dollar_str)
			free(dollar_str);
		(*i)++;
	}
	return (part);
}

static	t_word_part	*collect_word_parts(char *input, int *i)
{
	t_word_part	*parts;
	t_word_part	*new_part;

	parts = NULL;
	while (!is_word_boundary(input[*i]))
	{
		if (input[*i] == '\'')
			new_part = extract_single_quoted(input, i);
		else if (input[*i] == '"')
			new_part = extract_double_quoted(input, i);
		else if (input[*i] == '$')
			new_part = handle_dollar(input, i);
		else
			new_part = extract_literal(input, i);
		if (!add_word_part(&parts, new_part))
			return (NULL);
	}
	return (parts);
}

t_token	*create_word_token(char *input, int *i, int *pos)
{
	t_token		*token;
	t_word_part	*parts;

	parts = collect_word_parts(input, i);
	if (!parts)
		return (NULL);
	token = token_new_word(parts, *pos);
	if (!token)
	{
		word_part_list_free(parts);
		return (NULL);
	}
	(*pos)++;
	return (token);
}
