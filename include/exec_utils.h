/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsarraci <lsarraci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/29 17:23:52 by lsarraci          #+#    #+#             */
/*   Updated: 2026/01/29 17:30:23 by lsarraci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXEC_UTILS_H
# define EXEC_UTILS_H

# include "structs.h"

/* string verification utilities */
int		is_builtin(char *cmd);
int		is_valid_identifier(char *str);
int		is_numeric(char *str);
int		is_env_builtin(char *command);
int		is_redirect_needed(t_command *cmd);
int		is_pure_redirect_command_node(t_ast_node *node);
int		is_pure_redirect_command(t_command *cmd);

/* General utilities */
int		close_and_exit(int fd_in, int fd_out, int ex_status);
void	resolve_fd(int	*fd);
int		decide_command_type(char **args, t_env *env);
int		count_args(char **args);

/* file and path helpers kept here for convenience */
char	*define_executable(t_command *cmd, t_env *env);
void	ensure_redirect_files_created(t_command *cmd);

#endif
