/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   node_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsarraci <lsarraci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/22 16:15:27 by lsarraci          #+#    #+#             */
/*   Updated: 2025/12/22 19:11:47 by lsarraci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/shell.h"

t_ast_node	*node_new(void)
{
	t_ast_node	*node;

	node = malloc(sizeof(t_ast_node));
	if (!node)
		return (NULL);
	node->type = NODE_COMMAND;
	node->cmd = NULL;
	node->left = NULL;
	node->right = NULL;
	return (node);
}

void	node_free(t_ast_node *node)
{
	if (!node)
		return ;
	if (node->cmd)
		command_free(node->cmd);
	if (node->left)
		node_free(node->left);
	if (node->right)
		node_free(node->right);
	free(node);
}

t_ast_node	*node_new_command(t_command *cmd)
{
	t_ast_node	*node;

	node = node_new();
	if (!node)
		return (NULL);
	node->type = NODE_COMMAND;
	node->cmd = cmd;
	return (node);
}

t_ast_node	*node_new_operator(t_node_type type, t_ast_node *left,
		t_ast_node *right)
{
	t_ast_node	*node;

	node = node_new();
	if (!node)
		return (NULL);
	node->type = type;
	node->left = left;
	node->right = right;
	return (node);
}

int	command_add_arg(t_command *cmd, char *arg)
{
	int		count;
	char	**new_args;

	if (!cmd || !arg)
		return (0);
	count = 0;
	if (cmd->args)
	{
		while (cmd->args[count])
			count++;
	}
	new_args = ft_realloc(cmd->args, sizeof(char *) * (count + 1),
			sizeof(char *) * (count + 2));
	if (!new_args)
		return (0);
	new_args[count] = arg;
	new_args[count + 1] = NULL;
	cmd->args = new_args;
	return (1);
}
