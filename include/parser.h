/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsarraci <lsarraci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 15:11:15 by lsarraci          #+#    #+#             */
/*   Updated: 2025/12/23 18:22:59 by lsarraci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include "structs.h"

/* Command creation and manipulation */
t_command	*command_new(void);
void		command_free(t_command *cmd);
void		command_list_free(t_command *cmds);
void		command_list_add_back(t_command **list, t_command *new);
int			command_add_arg(t_command *cmd, char *arg);

/* redirect utils*/
t_redirect	*redirect_new(t_token_type type, char *file, char *delimiter);
void		redirect_free(t_redirect *redir);
void		redirect_list_free(t_redirect *redirects);
void		redirect_add_back(t_redirect **list, t_redirect *new);

/* heredoc utils */
int			should_expand_heredoc(char *delimiter);
char		*clear_heredoc_delimiter(char *delimiter);
char		*extract_var_name_heredoc(char *start, int *len);
char		*expand_heredoc_line(char *line);

/* heredoc processing */
int			process_heredoc(char *delimiter);

/* Syntax validation */
int			validate_syntax(t_token *tokens);
int			has_orphan_operator_start(t_token *tokens);
int			has_orphan_operator_end(t_token *tokens);
int			has_consecutive_operators(t_token *tokens);
int			has_redirect_without_target(t_token *tokens);
int			has_redirect_before_operator(t_token *tokens);

/* Error handling */
void		syntax_error(char *msg, t_token *token);
char		*get_token_type_name(t_token_type type);
void		parse_cleanup(t_ast_node *partial_tree, t_token *remaining_tokens);

/* Syntax helper checks */
int			is_invalid_start_token(t_token *token);
int			is_invalid_end_token(t_token *token);
t_token		*get_last_token(t_token *tokens);
int			redirect_has_valid_target(t_token *redirect);
int			is_invalid_redirect_sequence(t_token *current);

/* AST node creation and manipulation */
t_ast_node	*node_new(void);
t_ast_node	*node_new_command(t_command *cmd);
t_ast_node	*node_new_operator(t_node_type type, t_ast_node *left,
				t_ast_node *right);
void		node_free(t_ast_node *node);

/* General utility functions (used across multiple modules) */
char		*join_and_free(char *s1, char *s2);
char		*join_char_and_free(char *s, char c);
int			has_quotes(char c);
int			is_var_char(char c);
int			needs_expansion(char *str);

/* Expansion functions */
char		*expand_variable(char *var_name);
char		*expand_word(t_word_part *parts);

/* special variables */
int			is_special_var(char *var_name);
char		*expand_special_var(char *var_name);

/*parser utils */
int			is_word_or_redirect(t_token *token);
int			is_operator_token(t_token *token);
int			is_redirect(t_token *token);
int			handle_redirect(t_command *cmd, t_token **tokens);

/*parser main functions */
t_ast_node	*parse_tokens(t_token *tokens);
t_ast_node	*parse_logical(t_token **tokens);
t_ast_node	*parse_pipeline(t_token **tokens);
t_ast_node	*parse_simple_cmd(t_token **tokens);

#endif