/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_syntax.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsarraci <lsarraci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/23 17:48:01 by lsarraci          #+#    #+#             */
/*   Updated: 2025/12/23 18:17:11 by lsarraci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/shell.h"

int	validate_syntax(t_token *tokens)
{
	t_token	*last;

	if (!tokens)
		return (1);
	if (has_orphan_operator_start(tokens))
		return (syntax_error("unexpected token at start", tokens), 0);
	if (has_orphan_operator_end(tokens))
	{
		last = get_last_token(tokens);
		return (syntax_error("unexpected token at end", last), 0);
	}
	if (has_consecutive_operators(tokens))
		return (syntax_error("unexpected operator", NULL), 0);
	if (has_redirect_without_target(tokens))
		return (syntax_error("expected filename after redirect", NULL), 0);
	if (has_redirect_before_operator(tokens))
		return (syntax_error("expected filename before operator", NULL), 0);
	return (1);
}
