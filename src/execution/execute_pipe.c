/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipe.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsarraci <lsarraci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/02 14:56:26 by lsarraci          #+#    #+#             */
/*   Updated: 2026/01/23 16:53:53 by lsarraci         ###   ########.fr       */
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

int	execute_pipe(t_ast_node *node, t_env *env)
{
	pid_t	pid_left;
	pid_t	pid_right;
	int		pipe_fds[2];
	int		old_in_pipeline;

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
	pid_left = fork();
	if (pid_left == -1)
		return (perror("fork"), 1);
	if (pid_left == 0)
		setup_left_child(pipe_fds, node->left, env);
	pid_right = fork();
	if (pid_right == -1)
		return (perror("fork"), 1);
	if (pid_right == 0)
		setup_right_child(pipe_fds, node->right, env);
	close(pipe_fds[0]);
	close(pipe_fds[1]);
	env->in_pipeline = old_in_pipeline;
	return (wait_both_children(pid_left, pid_right));
}
