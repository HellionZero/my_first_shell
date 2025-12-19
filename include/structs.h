/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structs.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsarraci <lsarraci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 15:10:09 by lsarraci          #+#    #+#             */
/*   Updated: 2025/12/19 15:22:19 by lsarraci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCTS_H
# define STRUCTS_H

# include "types.h"

typedef struct s_token		t_token;
typedef struct s_command	t_command;
typedef struct s_word_part	t_word_part;

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

struct s_command
{
	char				**args;
	char				*infile;
	char				*outfile;
	char				*heredoc;
	int					append;
	struct s_command	*next;
}	;

#endif