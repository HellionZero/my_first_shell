/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsarraci <lsarraci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/23 17:56:03 by lsarraci          #+#    #+#             */
/*   Updated: 2025/12/23 18:00:25 by lsarraci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/shell.h"

int	has_orphan_operator_start(t_token *tokens)
{
	return (is_invalid_start_token(tokens));
}

int	has_orphan_operator_end(t_token *tokens)
{
	t_token	*last;

	last = get_last_token(tokens);
	return (is_invalid_end_token(last));
}

int	has_consecutive_operators(t_token *tokens)
{
	t_token	*current;

	current = tokens;
	while (current && current->next)
	{
		if (is_operator_token(current) && is_operator_token(current->next))
			return (1);
		current = current->next;
	}
	return (0);
}

int	has_redirect_without_target(t_token *tokens)
{
	t_token	*current;

	current = tokens;
	while (current)
	{
		if (is_redirect(current) && !redirect_has_valid_target(current))
			return (1);
		current = current->next;
	}
	return (0);
}

int	has_redirect_before_operator(t_token *tokens)
{
	t_token	*current;

	current = tokens;
	while (current && current->next)
	{
		if (is_invalid_redirect_sequence(current))
			return (1);
		current = current->next;
	}
	return (0);
}
