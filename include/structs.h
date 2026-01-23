/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structs.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsarraci <lsarraci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 15:10:09 by lsarraci          #+#    #+#             */
/*   Updated: 2026/01/23 18:15:37 by lsarraci         ###   ########.fr       */
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
typedef struct s_exec_context	t_exec_context;
typedef struct s_signal_state	t_signal_state;
typedef struct sigaction		t_sigaction;
typedef struct stat				t_stat;
typedef struct s_pipe_pids		t_pipe_pids;

struct	s_signal_state {
	volatile sig_atomic_t	in_heredoc;
	volatile sig_atomic_t	in_execution;
	volatile sig_atomic_t	received;
}	;

struct s_token
{
	t_token_type	type;
	t_word_part		*parts;
	char			*value;
	int				pos;
	struct s_token	*next;
}	;

struct s_word_part
{
	t_word_part_type	type;
	char				*content;
	struct s_word_part	*next;
}	;

struct s_redirect
{
	t_token_type		type;
	char				*file;
	char				*delimiter;
	int					heredoc_fd;
	struct s_redirect	*next;
}	;

struct s_command
{
	char				**args;
	t_redirect			*redirects;
	struct s_command	*next;
}	;

struct s_ast_node
{
	t_node_type			type;
	t_command			*cmd;
	struct s_ast_node	*left;
	struct s_ast_node	*right;
}	;

struct s_env_var
{
	char				*key;
	char				*value;
	struct s_env_var	*next;
}	;

struct s_env
{
	t_env_var			*vars;
	int					last_exit_status;
	int					should_exit;
	int					exit_code;
	int					in_pipeline;
}	;

struct s_exec_context
{
	t_ast_node			*tree_root;
	t_token				*tokens;
	t_env				*env;
}	;

struct s_pipe_pids
{
	pid_t	left;
	pid_t	right;
}	;

#endif