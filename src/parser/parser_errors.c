/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_errors.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsarraci <lsarraci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/23 17:50:05 by lsarraci          #+#    #+#             */
/*   Updated: 2025/12/23 18:21:18 by lsarraci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/shell.h"

char	*get_token_type_name(t_token_type type)
{
	if (type == TOKEN_PIPE)
		return ("|");
	if (type == TOKEN_AND)
		return ("&&");
	if (type == TOKEN_OR)
		return ("||");
	if (type == TOKEN_REDIR_IN)
		return ("<");
	if (type == TOKEN_REDIR_OUT)
		return (">");
	if (type == TOKEN_APPEND)
		return (">>");
	if (type == TOKEN_HEREDOC)
		return ("<<");
	if (type == TOKEN_WORD)
		return ("word");
	return ("unknown");
}

void	syntax_error(char *msg, t_token *token)
{
	ft_putstr_fd("minishell: syntax error: ", STDERR_FILENO);
	ft_putstr_fd(msg, STDERR_FILENO);
	if (token)
	{
		ft_putstr_fd(" near `", STDERR_FILENO);
		ft_putstr_fd(get_token_type_name(token->type), STDERR_FILENO);
		ft_putstr_fd("'", STDERR_FILENO);
	}
	ft_putstr_fd("\n", STDERR_FILENO);
	set_exit_status(2);
}

void	parse_cleanup(t_ast_node *partial_tree, t_token *remaining_tokens)
{
	if (partial_tree)
		node_free(partial_tree);
	if (remaining_tokens)
		token_list_free(remaining_tokens);
}
