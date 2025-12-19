/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_arrows.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsarraci <lsarraci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/19 14:09:20 by lsarraci          #+#    #+#             */
/*   Updated: 2025/12/19 14:22:42 by lsarraci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/shell.h"

static t_token	*create_redirect_in_token(char *input, int *i, int pos)
{
	if (input[*i + 1] == '<')
		return (create_single_token("<<", TOKEN_HEREDOC, i, pos));
	else
		return (create_single_token("<", TOKEN_REDIR_IN, i, pos));
}

static t_token	*create_redirect_out_token(char *input, int *i, int pos)
{
	if (input[*i + 1] == '>')
		return (create_single_token(">>", TOKEN_APPEND, i, pos));
	else
		return (create_single_token(">", TOKEN_REDIR_OUT, i, pos));
}

t_token	*create_arrow_token(char *input, int *i, int *pos)
{
	t_token	*token;

	token = NULL;
	if (input[*i] == '<')
		token = create_redirect_in_token(input, i, *pos);
	else if (input[*i] == '>')
		token = create_redirect_out_token(input, i, *pos);
	if (!token)
		return (NULL);
	(*pos)++;
	return (token);
}
