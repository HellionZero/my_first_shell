/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_command.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsarraci <lsarraci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/22 17:28:26 by lsarraci          #+#    #+#             */
/*   Updated: 2026/01/05 14:33:59 by lsarraci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/shell.h"

static int	process_word(t_command *cmd, t_token *token)
{
	char	*expanded;
	int		has_quotes;

	expanded = expand_word(token->parts);
	if (!expanded)
		return (0);
	has_quotes = has_literal_quote(token->parts);
	if (!should_keep_expanded_arg(expanded, has_quotes))
		return (free(expanded), 1);
	if (!command_add_arg(cmd, expanded))
		return (free(expanded), 0);
	return (1);
}

static int	process_token(t_command *cmd, t_token **tokens)
{
	if ((*tokens)->type == TOKEN_WORD)
		return (process_word(cmd, *tokens));
	if (is_redirect(*tokens))
		return (handle_redirect(cmd, tokens));
	return (1);
}

t_ast_node	*parse_simple_cmd(t_token **tokens)
{
	t_command	*cmd;

	cmd = command_new();
	if (!cmd)
		return (NULL);
	while (*tokens && is_word_or_redirect(*tokens))
	{
		if (!process_token(cmd, tokens))
			return (parse_error_command("invalid token", cmd), NULL);
		*tokens = (*tokens)->next;
	}
	if (!is_valid_command(cmd))
		return (empty_command_error(cmd), NULL);
	if (!process_all_heredocs(cmd))
	{
		if (get_signal_state()->received != SIGINT)
			parse_error_command("heredoc processing failed", cmd);
		else
			command_free(cmd);
		return (NULL);
	}
	return (node_new_command(cmd));
}
