/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsarraci <lsarraci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/17 16:18:45 by lsarraci          #+#    #+#             */
/*   Updated: 2025/12/17 19:22:53 by lsarraci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/shell.h"

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
			new = create_operator_token(input, &i, &pos);
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
