/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_commands.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsarraci <lsarraci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/17 18:56:10 by lsarraci          #+#    #+#             */
/*   Updated: 2025/12/17 19:22:02 by lsarraci         ###   ########.fr       */
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

static t_token	*create_logic_token(char *input, int *i, int pos)
{
	if (input[*i] == '&' && input[*i + 1] == '&')
		return (create_single_token("&&", TOKEN_AND, i, pos));
	return (NULL);
}

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

t_token	*create_operator_token(char *input, int *i, int *pos)
{
	t_token	*token;

	token = NULL;
	if (input[*i] == '|')
		token = create_pipe_token(input, i, *pos);
	else if (input[*i] == '&')
		token = create_logic_token(input, i, *pos);
	else if (input[*i] == '<')
		token = create_redirect_in_token(input, i, *pos);
	else if (input[*i] == '>')
		token = create_redirect_out_token(input, i, *pos);
	if (!token)
		return (NULL);
	(*pos)++;
	return (token);
}
