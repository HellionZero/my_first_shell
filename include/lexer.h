/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsarraci <lsarraci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 15:04:24 by lsarraci          #+#    #+#             */
/*   Updated: 2026/02/02 15:22:32 by lsarraci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
# define LEXER_H

# include "structs.h"

/*--------------- lexer utilities ---------------------------*/

/* verifies if the char is an operator */
int			is_operator(char c);

/* skips spaces in the input string from index i */
int			skip_spaces(char *input, int i);

/* checks if the char is part of a word */
int			is_word_char(char c);

/* checks if the char is a word boundary */
int			is_word_boundary(char c);

/* checks if the char is not a special char */
int			is_not_special(char c);

/* validates that all quotes in the input are properly closed */
int			validate_quotes(char *input);

/*--------------- variable utilities ------------------------*/

/* checks if the char is a special character ($, ?, *, #, @ and !) */
int			is_special_var_char(char c);

/* checks if the char is valid in a variable name */
int			is_var_name_char(char c);

/*------------------- token operations ----------------------*/

t_token		*create_single_token(char *op_string,
				t_token_type type, int *i, int pos);
t_token		*create_logical_token(char *input, int *i, int *pos);
t_token		*create_arrow_token(char *input, int *i, int *pos);
t_token		*create_word_token(char *input, int *i, int *pos);

/*--------------- word part extraction ----------------------*/
t_word_part	*extract_variable(char *input, int *i);
t_word_part	*extract_single_quoted(char *input, int *i);
t_word_part	*extract_double_quoted(char *input, int *i);
char		*extract_name_variable(char *input, int *i);
char		*extract_braced_variable(char *input, int *i);
char		*extract_positional_variable(char *input, int *i);
char		*extract_special_variable(char *input, int *i);

/* -------------Word part creation and manipulation----------*/
t_token		*token_new_word(t_word_part *parts, int pos);
t_word_part	*word_part_new(t_word_part_type type, char *content);
void		word_part_free(t_word_part	*part);
void		word_part_list_free(t_word_part	*parts);
void		word_part_add_back(t_word_part **list, t_word_part *new);

/* -------------Token creation and manipulation--------------*/
t_token		*token_new(t_token_type type, char *value, int pos);
void		token_free(t_token *token);
void		token_list_free(t_token *tokens);
void		token_add_back(t_token **list, t_token *new);

/* MAIN FUNCTION: responsible for tokenizing the input string */
t_token		*lexer(char *input, int i);

#endif