/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_child_process.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsarraci <lsarraci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/19 14:35:01 by lsarraci          #+#    #+#             */
/*   Updated: 2026/01/23 14:08:35 by lsarraci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/shell.h"

static int	execute_child_logicals(t_ast_node *node, t_env *env, int *exit_code)
{
	if (node->type == NODE_AND)
	{
		*exit_code = execute_logical_and(node->left, node->right, env);
		child_cleanup_and_exit(env, *exit_code);
	}
	if (node->type == NODE_OR)
	{
		*exit_code = execute_logical_or(node->left, node->right, env);
		child_cleanup_and_exit(env, *exit_code);
	}
	return (*exit_code);
}

static int	execute_child_pipes(t_ast_node *node, t_env *env, int *exit_code)
{
	if (node->type == NODE_PIPE)
	{
		*exit_code = execute_pipe(node, env);
		child_cleanup_and_exit(env, *exit_code);
	}
	return (*exit_code);
}

void	child_execute_node(t_ast_node *node, t_env *env)
{
	int	exit_code;

	exit_code = 0;
	if (!node)
		child_cleanup_and_exit(env, 1);
	if (node->type == NODE_AND || node->type == NODE_OR)
		exit_code = execute_child_logicals(node, env, &exit_code);
	else if (node->type == NODE_PIPE)
		exit_code = execute_child_pipes(node, env, &exit_code);
	else if (node->type == NODE_COMMAND && node->cmd && node->cmd->redirects)
	{
		if (!apply_redirects(node->cmd))
			child_cleanup_and_exit(env, 1);
		if (is_builtin(node->cmd->args[0]))
			child_execute_builtin(node->cmd, env);
		else
			child_execute_command(node->cmd, env);
	}
	else if (node->type == NODE_COMMAND && node->cmd)
	{
		if (is_builtin(node->cmd->args[0]))
			child_execute_builtin(node->cmd, env);
		else
			child_execute_command(node->cmd, env);
	}
	else
		child_execute_command(node->cmd, env);
}
