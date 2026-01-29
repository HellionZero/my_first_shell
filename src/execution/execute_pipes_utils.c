/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipes_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsarraci <lsarraci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/29 14:18:51 by lsarraci          #+#    #+#             */
/*   Updated: 2026/01/29 15:25:10 by lsarraci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/shell.h"

void	close_pipes(t_pipes *pipes)
{
	int	i;

	if (!pipes)
		return ;
	i = 0;
	while (i < pipes->n)
	{
		close(pipes->fds[i][0]);
		close(pipes->fds[i][1]);
		i++;
	}
}

static int	allocate_pipe_fds(t_pipes *pipes, int n_pipes)
{
	int	i;

	pipes->fds = malloc(sizeof * pipes->fds * n_pipes);
	if (! pipes->fds)
	{
		perror("malloc");
		return (0);
	}
	i = 0;
	while (i < n_pipes)
	{
		if (pipe(pipes->fds[i]) == -1)
		{
			perror("pipe");
			return (0);
		}
		i++;
	}
	pipes->n = n_pipes;
	return (1);
}

t_pipes	*create_pipes(int n_cmds)
{
	t_pipes	*pipes;
	int		n_pipes;

	if (n_cmds <= 1)
		return (NULL);
	n_pipes = n_cmds - 1;
	pipes = malloc(sizeof * pipes);
	if (! pipes)
	{
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	if (! allocate_pipe_fds(pipes, n_pipes))
	{
		free(pipes);
		exit(EXIT_FAILURE);
	}
	return (pipes);
}

void	free_pipes(t_pipes *pipes)
{
	if (!pipes)
		return ;
	free(pipes->fds);
	free(pipes);
}
