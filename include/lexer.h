/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsarraci <lsarraci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 15:04:24 by lsarraci          #+#    #+#             */
/*   Updated: 2026/02/05 16:30:22 by lsarraci         ###   ########.fr       */
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

/* creates a single token */
t_token		*create_single_token(char *op_string,
				t_token_type type, int *i, int pos);

/* creates a logical operator token */
t_token		*create_logical_token(char *input, int *i, int *pos);

/* creates a redirection token */
t_token		*create_arrow_token(char *input, int *i, int *pos);

/* creates a word token by extracting its parts */
t_token		*create_word_token(char *input, int *i, int *pos);

/*--------------- word part extraction ----------------------*/

/* locates a variable and extracts it as a word part */
t_word_part	*extract_variable(char *input, int *i);

/* extracts a single quoted string as a word part */
t_word_part	*extract_single_quoted(char *input, int *i);

/* extracts a double quoted string as a word part */
t_word_part	*extract_double_quoted(char *input, int *i);

/* extracts a literal string as a word part */
char		*extract_name_variable(char *input, int *i);

/* extracts a braced variable as a word part */
char		*extract_braced_variable(char *input, int *i);

/* extracts a positional variable as a word part 
a positional variable is a variable located inside 
a given position inside a variable. */
char		*extract_positional_variable(char *input, int *i);

/* extracts a special variable as a word part.
special variables are variables with a dollar 
sign followed by a special character. */
char		*extract_special_variable(char *input, int *i);

/* -------------Word part creation and manipulation----------*/

/* creates a new word token from word parts 
word parts are created to ensure the separation
between different components of a word and
to guarantee proper tokenization of words 
and variables */
t_token		*token_new_word(t_word_part *parts, int pos);

/* creates a new word part */
t_word_part	*word_part_new(t_word_part_type type, char *content);

/* frees a word part */
void		word_part_free(t_word_part	*part);

/* frees a list of word parts */
void		word_part_list_free(t_word_part	*parts);

/* adds a word part to the end of the list.
if there is no list, it creates one */
void		word_part_add_back(t_word_part **list, t_word_part *new);

/* -------------Token creation and manipulation--------------*/

/* creates a new token */
t_token		*token_new(t_token_type type, char *value, int pos);

/* frees a token */
void		token_free(t_token *token);

/* frees a list of tokens */
void		token_list_free(t_token *tokens);

/* adds a token to the end of the list.
if there is no list, it creates one */
void		token_add_back(t_token **list, t_token *new);

/* MAIN FUNCTION: responsible for tokenizing the input string */
t_token		*lexer(char *input, int i);

#endif