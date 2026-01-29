/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   types.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsarraci <lsarraci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 15:07:38 by lsarraci          #+#    #+#             */
/*   Updated: 2026/01/10 18:33:08 by lsarraci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TYPES_H
# define TYPES_H

typedef enum e_verbose				t_verbose;
typedef enum e_color_mode			t_color_mode;
typedef enum e_terminal_type		t_terminal_type;
typedef enum e_prompt_type			t_prompt_type;
typedef enum e_prompt_style			t_prompt_style;
typedef enum e_output_format		t_output_format;

typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_REDIR_IN,
	TOKEN_REDIR_OUT,
	TOKEN_APPEND,
	TOKEN_HEREDOC,
	TOKEN_SINGLE_QUOTE,
	TOKEN_DOUBLE_QUOTE,
	TOKEN_VARIABLE,
	TOKEN_AND,
	TOKEN_OR,
}	t_token_type;

typedef enum e_word_part_type
{
	PART_LITERAL,
	PART_SINGLE_QUOTE,
	PART_DOUBLE_QUOTE,
	PART_VARIABLE,
}	t_word_part_type;

typedef enum e_node_type
{
	NODE_COMMAND,
	NODE_PIPE,
	NODE_AND,
	NODE_OR,
}	t_node_type;

#endif