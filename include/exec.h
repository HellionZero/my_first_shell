/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsarraci <lsarraci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/02 14:52:17 by lsarraci          #+#    #+#             */
/*   Updated: 2026/01/29 17:26:34 by lsarraci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXEC_H
# define EXEC_H

# include "structs.h"
# include "exec_utils.h"
# include "pipeline.h"
# include "redirects.h"

/*--------- execuiton main functions ----------------------*/
int			execute_ast(t_ast_node *node, t_env *env);
int			execute_command(t_command *cmd, t_env *env);
int			execute_redirects(t_command *command, int fd, t_env *env);
int			execute_pipe(t_ast_node *node, t_env *env);
int			execute_logical_and(t_ast_node *left, t_ast_node *right,
				t_env *env);
int			execute_logical_or(t_ast_node *left, t_ast_node *right,
				t_env *env);
int			execute_external_command(char **args, t_env *env);

/* execute command helpers */
char		*define_executable(t_command *cmd, t_env *env);
char		*get_command_executable(t_command *cmd, t_env *env);
int			set_command_code(t_command *cmd, char **executable);
int			prepare_and_spawn_external(t_command *cmd, t_env *env);
int			fork_and_execute(t_command *cmd, char *executable, t_env *env);
void		ensure_redirect_files_created(t_command *cmd);

/*--------- file builtin executor ------------------------*/
int			execute_builtin_files(char **args, t_env *env);

/*--------- general builtin executor ---------------------*/
int			execute_builtin(char **args, t_env *env);

/*--------- child process functions ----------------------*/
int			wait_child(pid_t pid);
void		child_execute_builtin(t_command *cmd, t_env *env);
void		child_execute_command_node(t_ast_node *node, t_env *env);
void		child_execute_command(t_command *cmd, t_env *env);
void		child_exec_external(t_command *cmd, t_env *env);
void		child_cleanup_and_exit(t_env *env, int exit_code);

#endif