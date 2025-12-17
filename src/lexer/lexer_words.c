/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_words.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsarraci <lsarraci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/17 18:56:21 by lsarraci          #+#    #+#             */
/*   Updated: 2025/12/17 19:19:22 by lsarraci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/shell.h"

static char	*extract_word(char *input, int *i)
{
	char			*word;
	size_t			len;
	int				start;

	start = *i;
	while (input[*i] && is_word_char(input[*i]))
		(*i)++;
	len = *i - start;
	word = ft_substr(input, start, len);
	if (!word)
		return (NULL);
	return (word);
}

t_token	*create_word_token(char *input, int *i, int *pos)
{
	char	*word;
	t_token	*token;

	word = extract_word(input, i);
	if (!word)
		return (NULL);
	token = token_new(TOKEN_WORD, word, *pos);
	if (!token)
	{
		free(word);
		return (NULL);
	}
	(*pos)++;
	return (token);
}

t_token	*create_single_quote_token(char *input, int *i, int *pos)
{
	(void)input;
	(void)i;
	(void)pos;
	return (NULL);
}

t_token	*create_double_quote_token(char *input, int *i, int *pos)
{
	(void)input;
	(void)i;
	(void)pos;
	return (NULL);
}

t_token	*create_variable_token(char *input, int *i, int *pos)
{
	(void)input;
	(void)i;
	(void)pos;
	return (NULL);
}
