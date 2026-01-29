/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsarraci <lsarraci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/02 14:52:17 by lsarraci          #+#    #+#             */
/*   Updated: 2026/01/29 16:05:14 by lsarraci         ###   ########.fr       */
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

void		close_pipes(t_pipes *pipes);
int			wait_pipeline_children(pid_t *pids, int n_cmds);

t_pipes		*create_pipes(int n_cmds);
void		free_pipes(t_pipes *pipes);
/* Pipeline manager */
t_pipeline	*pipeline_create(t_ast_node *node);
int			pipeline_spawn(t_pipeline *pl, t_env *env);
int			pipeline_wait_and_cleanup(t_pipeline *pl);
void		pipeline_destroy(t_pipeline *pl);
/*--------- redirection utilities ----------------------*/
int			apply_redir_in(t_redirect *redir);
int			apply_redir_out(t_redirect *redir);
int			apply_append(t_redirect *redir);
int			apply_heredoc(t_redirect *redir);
int			apply_redirects(t_command *cmd);

/* internal redirect helpers (moved to utils) */
void		cleanup_unused_heredocs(t_command *cmd);
int			apply_single_redirect(t_redirect *redir);
int			validate_out_file(t_redirect *current, t_command *cmd);
int			validate_in_file(t_redirect *current, t_command *cmd);
int			validate_heredoc(t_redirect *current, t_command *cmd);
int			try_apply_redirects(t_command *cmd);
int			save_stdio(int *saved_stdin, int *saved_stdout);
int			process_redirect_item(t_redirect *current, t_command *cmd);
int			save_stdio(int *saved_stdin, int *saved_stdout);
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

/* execute command helpers */
char		*get_command_executable(t_command *cmd, t_env *env);
int			set_command_code(t_command *cmd, char **executable);
int			prepare_and_spawn_external(t_command *cmd, t_env *env);
int			fork_and_execute(t_command *cmd, char *executable, t_env *env);

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
pid_t		*spawn_pipeline_processes(t_ast_node **cmds,
				int n_cmds, t_pipes *pipes, t_env *env);

#endif