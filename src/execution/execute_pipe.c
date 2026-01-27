/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipe.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsarraci <lsarraci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/02 14:56:26 by lsarraci          #+#    #+#             */
/*   Updated: 2026/01/27 18:44:59 by lsarraci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/shell.h"

static int	count_pipeline_cmds(t_ast_node *node)
{
	int	count;

	count = 0;
	while (node && node->type == NODE_PIPE)
	{
		count++;
		node = node->left;
	}
	return (count + 1);
}

static void	fill_pipeline_cmds(t_ast_node *node, t_ast_node **cmds, int n)
{
	int	i;

	i = n - 1;
	while (node && node->type == NODE_PIPE)
	{
		cmds[i--] = node->right;
		node = node->left;
	}
	cmds[0] = node;
}

static int	(*prepare_pipes(int n_cmds))[2]
{
	int	(*pipes)[2];
	int	i;

	pipes = (int (*)[2])malloc(sizeof(int[2]) * (n_cmds - 1));
	i = 0;
	while (i < n_cmds - 1)
	{
		if (pipe(pipes[i]) == -1)
		{
			perror("pipe");
			exit(EXIT_FAILURE);
		}
		i++;
	}
	return (pipes);
}

int	execute_pipe(t_ast_node *node, t_env *env)
{
	int			n_cmds;
	t_ast_node	**cmds;
	int			(*pipes)[2];
	pid_t		*pids;
	int			i;
	int			status;

	n_cmds = count_pipeline_cmds(node);
	cmds = (t_ast_node **)malloc(sizeof(t_ast_node *) * n_cmds);
	fill_pipeline_cmds(node, cmds, n_cmds);
	pipes = prepare_pipes(n_cmds);
	pids = (pid_t *)malloc(sizeof(pid_t) * n_cmds);
	i = 0;
	while (i < n_cmds)
	{
		pids[i] = fork();
		if (pids[i] == -1)
		{
			perror("fork");
			exit(EXIT_FAILURE);
		}
		if (pids[i] == 0)
		{
			if (i > 0)
				dup2(pipes[i - 1][0], STDIN_FILENO);
			if (i < n_cmds - 1)
				dup2(pipes[i][1], STDOUT_FILENO);
			close_pipes(pipes, n_cmds - 1);
			if (is_pure_redirect_command_node(cmds[i]))
			{
				ensure_redirect_files_created(cmds[i]->cmd);
				child_cleanup_and_exit(env, 0);
			}
			child_execute_command_node(cmds[i], env);
			exit(EXIT_FAILURE);
		}
		i++;
	}
	close_pipes(pipes, n_cmds - 1);
	status = wait_pipeline_children(pids, n_cmds);
	free(pipes);
	free(cmds);
	return (status);
}
