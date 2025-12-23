/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structs.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsarraci <lsarraci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 15:10:09 by lsarraci          #+#    #+#             */
/*   Updated: 2025/12/22 17:36:44 by lsarraci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCTS_H
# define STRUCTS_H

# include "types.h"

typedef struct s_token		t_token;
typedef struct s_command	t_command;
typedef struct s_word_part	t_word_part;
typedef struct s_redirect	t_redirect;
typedef struct s_ast_node	t_ast_node;
typedef struct s_env		t_env_var;
typedef struct s_env		t_env;

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
	t_env_var	*vars;
	int			last_exit_status;
}	;

#endif