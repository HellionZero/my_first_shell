/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipeline.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsarraci <lsarraci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/29 16:00:10 by lsarraci          #+#    #+#             */
/*   Updated: 2026/01/29 16:11:23 by lsarraci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/shell.h"

static void	spawn_child_process(t_pipeline *pl, t_env *env, int i)
{
	pl->pids[i] = fork();
	if (pl->pids[i] == -1)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
	if (pl->pids[i] == 0)
	{
		if (i > 0 && pl->pipes)
			dup2(pl->pipes->fds[i - 1][0], STDIN_FILENO);
		if (i < pl->n_cmds - 1 && pl->pipes)
			dup2(pl->pipes->fds[i][1], STDOUT_FILENO);
		close_pipes(pl->pipes);
		if (is_pure_redirect_command_node(pl->cmds[i]))
		{
			ensure_redirect_files_created(pl->cmds[i]->cmd);
			child_cleanup_and_exit(env, 0);
		}
		child_execute_command_node(pl->cmds[i], env);
		exit(EXIT_FAILURE);
	}
}

int	pipeline_spawn(t_pipeline *pl, t_env *env)
{
	int	i;

	pl->pids = (pid_t *)malloc(sizeof(pid_t) * pl->n_cmds);
	if (!pl->pids)
	{
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	i = 0;
	while (i < pl->n_cmds)
	{
		spawn_child_process(pl, env, i);
		i++;
	}
	return (0);
}

int	pipeline_wait_and_cleanup(t_pipeline *pl)
{
	int	i;
	int	exit_status;
	int	final_status;

	if (pl->pipes)
		close_pipes(pl->pipes);
	i = 0;
	final_status = 0;
	while (i < pl->n_cmds)
	{
		waitpid(pl->pids[i], &exit_status, 0);
		if (WIFEXITED(exit_status))
			final_status = WEXITSTATUS(exit_status);
		else if (WIFSIGNALED(exit_status))
			final_status = 128 + WTERMSIG(exit_status);
		i++;
	}
	free(pl->pids);
	pl->pids = NULL;
	return (final_status);
}

void	pipeline_destroy(t_pipeline *pl)
{
	if (!pl)
		return ;
	if (pl->pipes)
		free_pipes(pl->pipes);
	free(pl->cmds);
	free(pl);
}
