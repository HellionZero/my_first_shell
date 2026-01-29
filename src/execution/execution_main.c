/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_main.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsarraci <lsarraci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/02 14:51:37 by lsarraci          #+#    #+#             */
/*   Updated: 2026/01/19 14:51:25 by lsarraci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/shell.h"

int	execute_ast(t_ast_node *node, t_env *env)
{
	if (!node)
		return (env->last_exit_status);
	if (node->type == NODE_COMMAND)
		return (execute_command(node->cmd, env));
	if (node->type == NODE_PIPE)
		return (execute_pipe(node, env));
	if (node->type == NODE_AND)
		return (execute_logical_and(node->left, node->right, env));
	if (node->type == NODE_OR)
		return (execute_logical_or(node->left, node->right, env));
	return (1);
}
