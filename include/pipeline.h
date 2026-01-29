/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsarraci <lsarraci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/29 17:25:29 by lsarraci          #+#    #+#             */
/*   Updated: 2026/01/29 17:31:32 by lsarraci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPELINE_H
# define PIPELINE_H

# include "structs.h"
# include <signal.h>

/* Pipe helpers */
void		close_pipes(t_pipes *pipes);
int			wait_pipeline_children(pid_t *pids, int n_cmds);
t_pipes		*create_pipes(int n_cmds);
void		free_pipes(t_pipes *pipes);

/* Pipeline manager */
t_pipeline	*pipeline_create(t_ast_node *node);
int			pipeline_spawn(t_pipeline *pl, t_env *env);
int			pipeline_wait_and_cleanup(t_pipeline *pl);
void		pipeline_destroy(t_pipeline *pl);
pid_t		*spawn_pipeline_processes(t_ast_node **cmds,
				int n_cmds, t_pipes *pipes, t_env *env);

#endif
