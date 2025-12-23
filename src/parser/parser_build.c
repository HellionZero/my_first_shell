/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_build.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsarraci <lsarraci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/22 17:59:31 by lsarraci          #+#    #+#             */
/*   Updated: 2025/12/22 19:11:53 by lsarraci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/shell.h"

t_ast_node	*parse_pipeline(t_token **tokens)
{
	t_ast_node	*left;
	t_ast_node	*right;

	left = parse_simple_cmd(tokens);
	if (!left)
		return (NULL);
	while (*tokens && (*tokens)->type == TOKEN_PIPE)
	{
		*tokens = (*tokens)->next;
		right = parse_simple_cmd(tokens);
		if (!right)
		{
			node_free(left);
			return (NULL);
		}
		left = node_new_operator(NODE_PIPE, left, right);
	}
	return (left);
}

t_ast_node	*parse_logical(t_token **tokens)
{
	t_ast_node	*left;
	t_ast_node	*right;
	t_node_type	op_type;

	left = parse_pipeline(tokens);
	if (!left)
		return (NULL);
	while (*tokens && ((*tokens)->type == TOKEN_AND
			|| (*tokens)->type == TOKEN_OR))
	{
		if ((*tokens)->type == TOKEN_AND)
			op_type = NODE_AND;
		else
			op_type = NODE_OR;
		*tokens = (*tokens)->next;
		right = parse_pipeline(tokens);
		if (!right)
		{
			node_free(left);
			return (NULL);
		}
		left = node_new_operator(op_type, left, right);
	}
	return (left);
}

t_ast_node	*parse_tokens(t_token *tokens)
{
	t_ast_node	*tree;

	if (!tokens)
		return (NULL);
	if (!validate_syntax(tokens))
		return (NULL);
	tree = parse_logical(&tokens);
	if (tokens != NULL)
	{
		syntax_error("unexpected tokens remaining", tokens);
		parse_cleanup(tree, NULL);
		return (NULL);
	}
	return (tree);
}
