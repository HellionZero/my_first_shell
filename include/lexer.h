/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsarraci <lsarraci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 15:04:24 by lsarraci          #+#    #+#             */
/*   Updated: 2025/12/19 18:29:28 by lsarraci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
# define LEXER_H

# include "structs.h"

/*--------------- lexer utilities ---------------------------*/
int			is_operator(char c);
int			skip_spaces(char *input, int i);
int			is_word_char(char c);
int			is_word_boundary(char c);
int			is_not_special(char c);

/*--------------- variable utilities ------------------------*/

int			is_special_var_char(char c);
int			is_var_name_char(char c);
char		*extract_name_variable(char *input, int *i);
char		*extract_braced_variable(char *input, int *i);
char		*extract_positional_variable(char *input, int *i);
char		*extract_special_variable(char *input, int *i);
t_word_part	*extract_variable(char *input, int *i);

/*------------------- token operations ----------------------*/

t_token		*create_single_token(char *op_string,
				t_token_type type, int *i, int pos);
t_token		*create_logical_token(char *input, int *i, int *pos);
t_token		*create_arrow_token(char *input, int *i, int *pos);
t_token		*create_word_token(char *input, int *i, int *pos);
t_word_part	*extract_single_quoted(char *input, int *i);
t_word_part	*extract_double_quoted(char *input, int *i);
t_token		*lexer(char *input);

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

#endif