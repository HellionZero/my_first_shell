/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsarraci <lsarraci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/02 14:52:17 by lsarraci          #+#    #+#             */
/*   Updated: 2026/02/05 16:17:02 by lsarraci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXEC_H
# define EXEC_H

# include "structs.h"
# include "exec_utils.h"
# include "pipeline.h"
# include "redirects.h"

/*--------- execuiton main functions ----------------------*/

/* MAIN FUNCTION: execute an abstract syntax tree */
int			execute_ast(t_ast_node *node, t_env *env);

/* function to execute a command.
can be placed in a child processes*/
int			execute_command(t_command *cmd, t_env *env);

/* function to execute redirects. */
int			execute_redirects(t_command *command, int fd, t_env *env);

/* function to execute a pipeline */
int			execute_pipe(t_ast_node *node, t_env *env);

/* function to execute logical AND */
int			execute_logical_and(t_ast_node *left, t_ast_node *right,
				t_env *env);

/* function to execute logical OR */
int			execute_logical_or(t_ast_node *left, t_ast_node *right,
				t_env *env);

/* function to execute external commands */
int			execute_external_command(char **args, t_env *env);

/* ------------ execute command helpers -------------------*/

/* function to define the executable path for a command */
char		*define_executable(t_command *cmd, t_env *env);

/* function to get the executable path for a command */
char		*get_command_executable(t_command *cmd, t_env *env);

/* function to set the command exit code. it's useful to
guarantees correct exit status handling */
int			set_command_code(t_command *cmd, char **executable);

/* function to prepare and spawn an external command */
int			prepare_and_spawn_external(t_command *cmd, t_env *env);

/* function to fork and execute a command. fork ensures 
each command runs in its own process. */
int			fork_and_execute(t_command *cmd, char *executable, t_env *env);

/* function to ensure redirect files are created before execution */
void		ensure_redirect_files_created(t_command *cmd);

/*--------- builtin executor functions ------------------------*/

/* function to execute builtins which
have file descriptor management */
int			execute_builtin_files(char **args, t_env *env);

/* function to execute builtins which
do not require file descriptor management */
int			execute_builtin(char **args, t_env *env);

/*--------- child process functions ----------------------*/

/* ensures the child process will end and cleanup */
int			wait_child(pid_t pid);

/* function to execute builtins in a child process */
void		child_execute_builtin(t_command *cmd, t_env *env);

/* function to execute a command in a child process */
void		child_execute_command(t_command *cmd, t_env *env);

/* function to execute a command node in a child process. 
ensure absence of empty nodes and envs, and decides between
execute a command or builtin if the arguments are valid */
void		child_execute_command_node(t_ast_node *node, t_env *env);

/* function to execute external commands in a child process.
The external commands are located in the system PATH. */
void		child_exec_external(t_command *cmd, t_env *env);

/* function to cleanup all resources and exit the child process */
void		child_cleanup_and_exit(t_env *env, int exit_code);

#endif