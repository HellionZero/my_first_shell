/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_checks.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsarraci <lsarraci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/23 18:03:55 by lsarraci          #+#    #+#             */
/*   Updated: 2025/12/23 18:04:08 by lsarraci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/shell.h"

int	is_invalid_start_token(t_token *token)
{
	if (!token)
		return (0);
	return (token->type == TOKEN_PIPE
		|| token->type == TOKEN_AND
		|| token->type == TOKEN_OR);
}

int	is_invalid_end_token(t_token *token)
{
	if (!token)
		return (0);
	return (token->type == TOKEN_PIPE
		|| token->type == TOKEN_AND
		|| token->type == TOKEN_OR);
}

t_token	*get_last_token(t_token *tokens)
{
	t_token	*last;

	if (!tokens)
		return (NULL);
	last = tokens;
	while (last->next)
		last = last->next;
	return (last);
}

int	redirect_has_valid_target(t_token *redirect)
{
	if (!redirect || !redirect->next)
		return (0);
	return (redirect->next->type == TOKEN_WORD);
}

int	is_invalid_redirect_sequence(t_token *current)
{
	if (!current || !current->next)
		return (0);
	return (is_redirect(current) && is_operator_token(current->next));
}
