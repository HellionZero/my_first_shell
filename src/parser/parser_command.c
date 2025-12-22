/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_command.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsarraci <lsarraci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/22 17:28:26 by lsarraci          #+#    #+#             */
/*   Updated: 2025/12/22 19:10:25 by lsarraci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/shell.h"

static int	process_word(t_command *cmd, t_token *token)
{
	char	*expanded;

	expanded = expand_word(token->parts);
	if (!expanded)
		return (0);
	if (!command_add_arg(cmd, expanded))
	{
		free(expanded);
		return (0);
	}
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
		{
			command_free(cmd);
			return (NULL);
		}
		*tokens = (*tokens)->next;
	}
	return (node_new_command(cmd));
}
