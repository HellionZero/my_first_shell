/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipeline_helpers.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsarraci <lsarraci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/27 17:34:24 by lsarraci          #+#    #+#             */
/*   Updated: 2026/01/29 16:14:30 by lsarraci         ###   ########.fr       */
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

t_pipeline	*pipeline_create(t_ast_node *node)
{
	t_pipeline	*pl;
	int			n_cmds;

	if (!node)
		return (NULL);
	n_cmds = count_pipeline_cmds(node);
	pl = (t_pipeline *)malloc(sizeof(t_pipeline));
	if (!pl)
	{
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	pl->n_cmds = n_cmds;
	pl->cmds = (t_ast_node **)malloc(sizeof(t_ast_node *) * n_cmds);
	if (!pl->cmds)
	{
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	fill_pipeline_cmds(node, pl->cmds, n_cmds);
	pl->pipes = create_pipes(n_cmds);
	pl->pids = NULL;
	return (pl);
}
