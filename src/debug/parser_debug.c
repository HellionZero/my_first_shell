/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_debug.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsarraci <lsarraci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/22 16:06:28 by lsarraci          #+#    #+#             */
/*   Updated: 2025/12/22 19:03:36 by lsarraci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/shell.h"

static const char	**get_node_type_names(void)
{
	static const char	*names[] = {
		"COMMAND",
		"PIPE",
		"AND",
		"OR"
	};

	return (names);
}

static void	print_tree_recursive(t_ast_node *node, int indent)
{
	if (!node)
		return ;
	print_indent(indent);
	ft_printf("Node: %s\n", get_node_type_names()[node->type]);
	if (node->type == NODE_COMMAND)
		print_command(node->cmd, indent + 1);
	else
	{
		if (node->left)
		{
			print_indent(indent);
			ft_printf("LEFT:\n");
			print_tree_recursive(node->left, indent + 1);
		}
		if (node->right)
		{
			print_indent(indent);
			ft_printf("RIGHT:\n");
			print_tree_recursive(node->right, indent + 1);
		}
	}
}

void	print_ast(t_ast_node *tree)
{
	ft_printf("\n=== ABSTRACT SYNTAX TREE ===\n");
	if (!tree)
	{
		ft_printf("(empty tree)\n");
		return ;
	}
	print_tree_recursive(tree, 0);
	ft_printf("===========================\n\n");
}
