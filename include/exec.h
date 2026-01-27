/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsarraci <lsarraci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/02 14:52:17 by lsarraci          #+#    #+#             */
/*   Updated: 2026/01/27 19:22:48 by lsarraci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXEC_H
# define EXEC_H

# include "structs.h"

/*--------- general utilities --------------------------*/
int			is_builtin(char *cmd);
int			is_valid_identifier(char *str);
int			count_args(char **args);
int			is_numeric(char *str);
int			is_env_builtin(char *command);
int			close_and_exit(int fd_in, int fd_out, int ex_status);
void		resolve_fd(int	*fd);
int			decide_command_type(char **args, t_env *env);
int			is_redirect_needed(t_command *cmd);
int			is_pure_redirect_command_node(t_ast_node *node);
int			is_pure_redirect_command(t_command *cmd);
void		close_pipes(int (*pipes)[2], int n_pipes);
int			wait_pipeline_children(pid_t *pids, int n_cmds);

/*--------- redirection utilities ----------------------*/
int			apply_redir_in(t_redirect *redir);
int			apply_redir_out(t_redirect *redir);
int			apply_append(t_redirect *redir);
int			apply_heredoc(t_redirect *redir);
int			apply_redirects(t_command *cmd);
int			empty_handle_manager(t_command *cmd, t_env *env);

/*--------- execuiton main functions ----------------------*/
int			execute_ast(t_ast_node *node, t_env *env);
int			execute_command(t_command *cmd, t_env *env);
int			execute_redirects(t_command *command, int fd, t_env *env);
int			execute_pipe(t_ast_node *node, t_env *env);
int			execute_logical_and(t_ast_node *left, t_ast_node *right,
				t_env *env);
int			execute_logical_or(t_ast_node *left, t_ast_node *right,
				t_env *env);
char		*define_executable(t_command *cmd, t_env *env);
int			execute_external_command(char **args, t_env *env);
void		ensure_redirect_files_created(t_command *cmd);

/*--------- file builtin executor ------------------------*/
int			execute_builtin_files(char **args, t_env *env);

/*--------- general builtin executor ---------------------*/
int			execute_builtin(char **args, t_env *env);

/*--------- child process functions ----------------------*/
void		child_execute_builtin(t_command *cmd, t_env *env);
void		child_execute_command_node(t_ast_node *node, t_env *env);
void		child_execute_command(t_command *cmd, t_env *env);
void		child_execute_node(t_ast_node *node, t_env *env);
void		child_cleanup_and_exit(t_env *env, int exit_code);

#endif