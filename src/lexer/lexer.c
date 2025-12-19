/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsarraci <lsarraci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/17 16:18:45 by lsarraci          #+#    #+#             */
/*   Updated: 2025/12/19 14:24:01 by lsarraci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/shell.h"

static t_token	*handle_operator(char *input, int *i, int *pos)
{
	if (input[*i] == '<' || input[*i] == '>')
		return (create_arrow_token(input, i, pos));
	else if (input[*i] == '|' || input[*i] == '&')
		return (create_logical_token(input, i, pos));
	return (NULL);
}

t_token	*lexer(char *input)
{
	int		i;
	int		pos;
	t_token	*tokens;
	t_token	*new;

	i = 0;
	tokens = NULL;
	pos = 0;
	while (input[i])
	{
		i = skip_spaces(input, i);
		if (!input[i])
			break ;
		if (is_operator(input[i]))
			new = handle_operator(input, &i, &pos);
		else
			new = create_word_token(input, &i, &pos);
		if (!new)
		{
			token_list_free(tokens);
			return (NULL);
		}
		token_add_back(&tokens, new);
	}
	return (tokens);
}
