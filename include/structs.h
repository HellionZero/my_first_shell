/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structs.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsarraci <lsarraci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 15:10:09 by lsarraci          #+#    #+#             */
/*   Updated: 2025/12/16 17:33:01 by lsarraci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCTS_H
# define STRUCTS_H

# include "types.h"

typedef struct s_token		t_token;
typedef struct s_command	t_command;

struct s_token
{
	t_token_type	type;
	char			*value;
	int				pos;
	struct s_token	*next;	
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