/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_logical.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsarraci <lsarraci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/02 14:57:22 by lsarraci          #+#    #+#             */
/*   Updated: 2026/01/10 14:46:07 by lsarraci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/shell.h"

static int	wait_child_status(pid_t pid)
{
	int	status;

	waitpid(pid, &status, 0);
	setup_signals_interactive();
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	return (1);
}

static int	fork_and_execute_node(t_ast_node *node, t_env *env)
{
	pid_t	pid;

	setup_signals_executing();
	pid = fork();
	if (pid == -1)
		return (perror("fork"), 1);
	if (pid == 0)
	{
		restore_signals_default();
		child_execute_node(node, env);
	}
	return (wait_child_status(pid));
}

int	execute_logical_and(t_ast_node *left, t_ast_node *right, t_env *env)
{
	int	left_status;

	if (!left || !right)
		return (1);
	left_status = fork_and_execute_node(left, env);
	if (left_status == 0)
		return (fork_and_execute_node(right, env));
	return (left_status);
}

int	execute_logical_or(t_ast_node *left, t_ast_node *right, t_env *env)
{
	int	left_status;

	if (!left || !right)
		return (1);
	left_status = fork_and_execute_node(left, env);
	if (left_status != 0)
		return (fork_and_execute_node(right, env));
	return (left_status);
}
