/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipe.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsarraci <lsarraci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/02 14:56:26 by lsarraci          #+#    #+#             */
/*   Updated: 2026/01/23 18:15:20 by lsarraci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/shell.h"

static void	setup_left_child(int *pipe_fds, t_ast_node *node, t_env *env)
{
	close(pipe_fds[0]);
	dup2(pipe_fds[1], STDOUT_FILENO);
	close(pipe_fds[1]);
	child_execute_node(node, env);
}

static void	setup_right_child(int *pipe_fds, t_ast_node *node, t_env *env)
{
	close(pipe_fds[1]);
	dup2(pipe_fds[0], STDIN_FILENO);
	close(pipe_fds[0]);
	child_execute_node(node, env);
}

static int	wait_both_children(pid_t pid_left, pid_t pid_right)
{
	int	status_left;
	int	status_right;

	waitpid(pid_left, &status_left, 0);
	waitpid(pid_right, &status_right, 0);
	setup_signals_interactive();
	if (WIFEXITED(status_right))
		return (WEXITSTATUS(status_right));
	if (WIFSIGNALED(status_right))
		return (128 + WTERMSIG(status_right));
	return (1);
}

static void	setup_children(int *pipe_fds, t_ast_node *node,
				t_env *env, t_pipe_pids *pids)
{
	pids->left = fork();
	if (pids->left == -1)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
	if (pids->left == 0)
		setup_left_child(pipe_fds, node->left, env);
	pids->right = fork();
	if (pids->right == -1)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
	if (pids->right == 0)
		setup_right_child(pipe_fds, node->right, env);
}

int	execute_pipe(t_ast_node *node, t_env *env)
{
	t_pipe_pids		pids;
	int				pipe_fds[2];
	int				old_in_pipeline;

	old_in_pipeline = env->in_pipeline;
	env->in_pipeline = 1;
	if (!node || !node->left || !node->right)
		return (1);
	setup_signals_executing();
	if (pipe(pipe_fds) == -1)
	{
		perror("pipe");
		return (1);
	}
	setup_children(pipe_fds, node, env, &pids);
	close(pipe_fds[0]);
	close(pipe_fds[1]);
	env->in_pipeline = old_in_pipeline;
	return (wait_both_children(pids.left, pids.right));
}
