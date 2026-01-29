/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_logical.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsarraci <lsarraci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/17 18:56:10 by lsarraci          #+#    #+#             */
/*   Updated: 2026/01/11 16:35:11 by lsarraci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/shell.h"

static t_token	*create_pipe_token(char *input, int *i, int pos)
{
	if (input[*i + 1] == '|')
		return (create_single_token("||", TOKEN_OR, i, pos));
	else
		return (create_single_token("|", TOKEN_PIPE, i, pos));
}

static t_token	*create_and_token(char *input, int *i, int pos)
{
	if (input[*i] == '&' && input[*i + 1] == '&')
		return (create_single_token("&&", TOKEN_AND, i, pos));
	return (NULL);
}

t_token	*create_logical_token(char *input, int *i, int *pos)
{
	t_token	*token;

	token = NULL;
	if (input[*i] == '|')
		token = create_pipe_token(input, i, *pos);
	else if (input[*i] == '&')
		token = create_and_token(input, i, *pos);
	if (!token)
		return (NULL);
	(*pos)++;
	return (token);
}
