/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsarraci <lsarraci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/29 17:25:29 by lsarraci          #+#    #+#             */
/*   Updated: 2026/02/05 15:40:49 by lsarraci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPELINE_H
# define PIPELINE_H

# include "structs.h"
# include <signal.h>

/* ---------------- Pipe helpers -------------*/

/* function to create pipes */
t_pipes		*create_pipes(int n_cmds);

/* function to close pipes */
void		close_pipes(t_pipes *pipes);

/* function to free pipes */
void		free_pipes(t_pipes *pipes);

/* ---------------- Pipeline manager -------------*/

/* function to create a pipeline from an AST node */
t_pipeline	*pipeline_create(t_ast_node *node);

/* function to destroy a pipeline */
int			pipeline_spawn(t_pipeline *pl, t_env *env);

/* function to wait for pipeline completion and cleanup */
int			pipeline_wait_and_cleanup(t_pipeline *pl);

/* function to destroy a pipeline */
void		pipeline_destroy(t_pipeline *pl);

#endif
