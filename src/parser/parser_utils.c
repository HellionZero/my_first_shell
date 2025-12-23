/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsarraci <lsarraci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/22 17:20:52 by lsarraci          #+#    #+#             */
/*   Updated: 2025/12/23 18:10:02 by lsarraci         ###   ########.fr       */
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

static void	define_redirect(t_command *cmd, t_token_type type, char *file)
{
	t_redirect	*redir;
	char		*delimiter;

	delimiter = NULL;
	if (type == TOKEN_HEREDOC)
	{
		delimiter = file;
		file = NULL;
	}
	redir = redirect_new(type, file, delimiter);
	if (!redir)
	{
		if (file)
			free(file);
		if (delimiter)
			free(delimiter);
		return ;
	}
	redirect_add_back(&cmd->redirects, redir);
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
	if (!file || file[0] == '\0')
	{
		if (file)
			free(file);
		return (0);
	}
	define_redirect(cmd, type, file);
	return (1);
}
