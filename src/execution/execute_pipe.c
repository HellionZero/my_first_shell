/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipe.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsarraci <lsarraci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/02 14:56:26 by lsarraci          #+#    #+#             */
/*   Updated: 2026/01/29 15:37:16 by lsarraci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/shell.h"

int	is_pure_redirect_command(t_command *cmd)
{
	if (!cmd)
		return (0);
	if ((!cmd->args || !cmd->args[0]) && cmd->redirects)
		return (1);
	return (0);
}

int	is_pure_redirect_command_node(t_ast_node *node)
{
	if (!node || !node->cmd)
		return (0);
	return (is_pure_redirect_command(node->cmd));
}

int	execute_pipe(t_ast_node *node, t_env *env)
{
	t_pipeline	*pl;
	int			status;

	pl = pipeline_create(node);
	if (!pl)
		return (1);
	if (pipeline_spawn(pl, env) == -1)
	{
		pipeline_destroy(pl);
		return (1);
	}
	status = pipeline_wait_and_cleanup(pl);
	pipeline_destroy(pl);
	return (status);
}
