/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipeline_helpers.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsarraci <lsarraci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/27 17:34:24 by lsarraci          #+#    #+#             */
/*   Updated: 2026/01/27 19:04:17 by lsarraci         ###   ########.fr       */
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

void	close_pipes(int (*pipes)[2], int n_pipes)
{
	int	i;

	i = 0;
	while (i < n_pipes)
	{
		close(pipes[i][0]);
		close(pipes[i][1]);
		i++;
	}
}

int	wait_pipeline_children(pid_t *pids, int n_cmds)
{
	int	i;
	int	exit_status;
	int	final_status;

	i = 0;
	final_status = 0;
	while (i < n_cmds)
	{
		waitpid(pids[i], &exit_status, 0);
		if (WIFEXITED(exit_status))
			final_status = WEXITSTATUS(exit_status);
		else if (WIFSIGNALED(exit_status))
			final_status = 128 + WTERMSIG(exit_status);
		i++;
	}
	free(pids);
	return (final_status);
}
