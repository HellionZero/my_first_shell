/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_command_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsarraci <lsarraci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/27 18:13:42 by lsarraci          #+#    #+#             */
/*   Updated: 2026/01/27 18:30:34 by lsarraci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/shell.h"

void	child_execute_command_node(t_ast_node *node, t_env *env)
{
	if (!node || !node->cmd)
		child_cleanup_and_exit(env, 1);
	if (is_pure_redirect_command(node->cmd))
	{
		ensure_redirect_files_created(node->cmd);
		child_cleanup_and_exit(env, 0);
	}
	if (node->cmd->args && node->cmd->args[0] && node->cmd->args[0][0] != '\0')
	{
		if (is_builtin(node->cmd->args[0]))
			child_execute_builtin(node->cmd, env);
		else
			child_execute_command(node->cmd, env);
	}
	else
		child_cleanup_and_exit(env, 0);
}
