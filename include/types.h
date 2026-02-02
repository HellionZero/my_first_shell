/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   types.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsarraci <lsarraci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 15:07:38 by lsarraci          #+#    #+#             */
/*   Updated: 2026/02/02 15:01:00 by lsarraci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TYPES_H
# define TYPES_H

/* Enum for identifying token types in the shell input */
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

/* Enum for identifying parts of a word in the shell input */
typedef enum e_word_part_type
{
	PART_LITERAL,
	PART_SINGLE_QUOTE,
	PART_DOUBLE_QUOTE,
	PART_VARIABLE,
}	t_word_part_type;

/* Enum for identifying AST node types */
typedef enum e_node_type
{
	NODE_COMMAND,
	NODE_PIPE,
	NODE_AND,
	NODE_OR,
}	t_node_type;

#endif