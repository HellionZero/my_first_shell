/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structs.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsarraci <lsarraci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 15:10:09 by lsarraci          #+#    #+#             */
/*   Updated: 2026/02/02 14:59:29 by lsarraci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCTS_H
# define STRUCTS_H

# include "types.h"

typedef struct s_token			t_token;
typedef struct s_command		t_command;
typedef struct s_word_part		t_word_part;
typedef struct s_redirect		t_redirect;
typedef struct s_ast_node		t_ast_node;
typedef struct s_env_var		t_env_var;
typedef struct s_env			t_env;
typedef struct s_signal_state	t_signal_state;
typedef struct sigaction		t_sigaction;
typedef struct stat				t_stat;
typedef struct s_pipe_pids		t_pipe_pids;
typedef struct s_pipes			t_pipes;
typedef struct s_pipeline		t_pipeline;

/* Signal state structure. 
is composed of flags indicating 
the shell's signal handling state */
struct	s_signal_state {
	volatile sig_atomic_t	in_heredoc;
	volatile sig_atomic_t	in_execution;
	volatile sig_atomic_t	received;
}	;

/* Token structure. Represents a lexical token in the shell input. 
type: the type of token, determined by an enum.
*parts: the parts that make up the token, linked list of word parts. 
*value: the string value of the token. 
pos: the position of the token in the input string. 
*next: pointer to the next token in the list. */
struct s_token
{
	t_token_type	type;
	t_word_part		*parts;
	char			*value;
	int				pos;
	struct s_token	*next;
}	;

/* Struct to handle parts of a word in a token. 
it's used to represent segments of a token's value,
such as literals or variables 
type: enum to classify the type of word part 
*content: string content of the word part 
*next: pointer to the next word part in the list. */
struct s_word_part
{
	t_word_part_type	type;
	char				*content;
	struct s_word_part	*next;
}	;

/* Redirect structure. Represents input/output redirection in a command. 
type: enum to represent the type
of redirection (e.g., input, output, append, heredoc) 
*file: the target file for the redirection 
*delimiter: the delimiter string for heredoc redirection, 
i.e., the string that ends the heredoc input.
*heredoc_fd: file descriptor for the heredoc input.
it's used to read the heredoc content. 
*next: pointer to the next redirect in the list. */
struct s_redirect
{
	t_token_type		type;
	char				*file;
	char				*delimiter;
	int					heredoc_fd;
	struct s_redirect	*next;
}	;

/* Struct representing a command in the shell. 
each command is a node in a linked list. 
**args: array of argument strings for the command.
*t_redirect: linked list of redirections associated with the command. 
*next: pointer to the next command in the list. */
struct s_command
{
	char				**args;
	t_redirect			*redirects;
	struct s_command	*next;
}	;

/* Binary tree node representing a command
or operation in the shell's abstract syntax tree.
the nodes are composed of the parsed commands and operations. 
type: enum to identify the type of node.
*cmd: pointer to the command structure, if the node represents a command.
*left: pointer to the left child node in the AST.
*right: pointer to the right child node in the AST.
*/
struct s_ast_node
{
	t_node_type			type;
	t_command			*cmd;
	struct s_ast_node	*left;
	struct s_ast_node	*right;
}	;

/* Structure representing an environment variable as a key-value pair. */
struct s_env_var
{
	char				*key;
	char				*value;
	struct s_env_var	*next;
}	;

/* Structure representing the shell's environment, 
including variables and state flags. 
it's composed of environment variables and control flags.
*vars: key-value pairs representing environment variables.
last_exit_status: the exit status of the last executed command.
should_exit: flag indicating if the shell should exit.
exit_code: the exit code to use when the shell exits.
in_pipeline: flag indicating if the shell is currently
executing within a pipeline, which is a sequence 
of commands connected by pipes. */
struct s_env
{
	t_env_var			*vars;
	int					last_exit_status;
	int					should_exit;
	int					exit_code;
	int					in_pipeline;
}	;

/* Structure representing a collection of pipes used in a pipeline. 
the first variable represents the array of
file descriptor pairs for the pipes.
the second variable represents the number of pipes. 
file descriptors are used for inter-process communication in the pipeline. */
struct s_pipes
{
	int	(*fds)[2];
	int	n;
}	;

/* Structure representing process IDs for
the left and right commands in a pipeline. 
each process in the pipeline represents 
a different child process with its own process ID
and the main process need to keep track 
of these IDs for management purposes. */
struct s_pipe_pids
{
	pid_t	left;
	pid_t	right;
}	;

/* Structure representing a pipeline of commands 
connected by pipes. It's used to manage the execution
of multiple commands in a pipeline, to handle 
inter-process communication and synchronization,
and is responsible for handle a large number of pipes at once. */
struct s_pipeline
{
	t_pipes		*pipes;
	t_ast_node	**cmds;
	pid_t		*pids;
	int			n_cmds;
};

#endif