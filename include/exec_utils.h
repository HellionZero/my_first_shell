/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsarraci <lsarraci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/29 17:23:52 by lsarraci          #+#    #+#             */
/*   Updated: 2026/02/05 17:32:00 by lsarraci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXEC_UTILS_H
# define EXEC_UTILS_H

# include "structs.h"

/* ------------- string verification utilities -----------------------*/

/* checks if command is a builtin */
int		is_builtin(char *cmd);

/* checks if the string is a valid identifier for env variables */
int		is_valid_identifier(char *str);

/* checks if the string is numeric */
int		is_numeric(char *str);

/* checks if the builtin is env related */
int		is_env_builtin(char *command);

/* checks if redirection is needed for the command */
int		is_redirect_needed(t_command *cmd);

/* checks if the command node is purely a redirection */
int		is_pure_redirect_command_node(t_ast_node *node);

/* checks if the command is purely a redirection */
int		is_pure_redirect_command(t_command *cmd);

/* -----------------General utilities ----------------- */

/* closes file descriptors and exits with the given status */
int		close_and_exit(int fd_in, int fd_out, int ex_status);

/* resolves file descriptor. it ensures the command
has a valid fd, duplicating it if there's none */
void	resolve_fd(int	*fd);

/* decides the command type and executes it accordingly */
int		decide_command_type(char **args, t_env *env);

/* counts the number of arguments in the args array */
int		count_args(char **args);

/*  --- file and path helpers kept here for convenience --- */

/* defines the executable path for a command */
char	*define_executable(t_command *cmd, t_env *env);

/* ensures that redirect files are created */
void	ensure_redirect_files_created(t_command *cmd);

#endif
