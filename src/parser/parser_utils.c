/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsarraci <lsarraci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/22 17:20:52 by lsarraci          #+#    #+#             */
/*   Updated: 2025/12/22 17:57:10 by lsarraci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/shell.h"

int	is_word_or_redirect(t_token *token)
{
	if (!token)
		return (0);
	if (token->type == TOKEN_WORD)
		return (1);
	if (token->type == TOKEN_REDIR_IN)
		return (1);
	if (token->type == TOKEN_REDIR_OUT)
		return (1);
	if (token->type == TOKEN_APPEND)
		return (1);
	if (token->type == TOKEN_HEREDOC)
		return (1);
	return (0);
}

int	is_operator_token(t_token *token)
{
	if (!token)
		return (0);
	return (token->type == TOKEN_PIPE
		|| token->type == TOKEN_AND
		|| token->type == TOKEN_OR);
}

int	is_redirect(t_token *token)
{
	if (!token)
		return (0);
	return (token->type == TOKEN_REDIR_IN
		|| token->type == TOKEN_REDIR_OUT
		|| token->type == TOKEN_APPEND
		|| token->type == TOKEN_HEREDOC);
}

int	handle_redirect(t_command *cmd, t_token **tokens)
{
	t_token_type	type;
	char			*file;

	if (!cmd || !tokens || !*tokens)
		return (0);
	type = (*tokens)->type;
	*tokens = (*tokens)->next;
	if (!*tokens || (*tokens)->type != TOKEN_WORD)
		return (0);
	file = expand_word((*tokens)->parts);
	if (!file)
		return (0);
	if (type == TOKEN_REDIR_IN)
		cmd->infile = file;
	else if (type == TOKEN_REDIR_OUT)
		cmd->outfile = file;
	else if (type == TOKEN_APPEND)
	{
		cmd->outfile = file;
		cmd->append = 1;
	}
	return (1);
}
