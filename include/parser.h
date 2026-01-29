/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsarraci <lsarraci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 15:11:15 by lsarraci          #+#    #+#             */
/*   Updated: 2026/01/11 16:49:37 by lsarraci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include "structs.h"

/* Command creation and manipulation */
int			is_valid_command(t_command *cmd);
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
void		write_line_to_pipe(int fd, char *line);

/* heredoc expansion utils */
void		read_heredoc_content(int pipe_fd, char *delimiter,
				char *clean_delim);
char		*get_expanded_line(char *line, char *delimiter);
int			is_delimiter_reached(char *line, char *clean_delim);

/* heredoc processing */
int			process_heredoc(char *delimiter);

/* heredoc file utils */
void		init_heredoc_fd(t_redirect *redir);
int			setup_heredoc(t_redirect *redir);
void		close_heredoc_fd(t_redirect *redir);
void		cleanup_heredoc_fds(t_redirect *redir);
int			process_all_heredocs(t_command *cmd);

/* environment variable setting*/

void		set_shell_env(t_env	*env);
t_env		*get_shell_env(void);
void		reset_shell_env(void);

/* Syntax validation */
int			validate_syntax(t_token *tokens);
int			has_orphan_operator_start(t_token *tokens);
int			has_orphan_operator_end(t_token *tokens);
int			has_consecutive_operators(t_token *tokens);
int			has_redirect_without_target(t_token *tokens);
int			has_redirect_before_operator(t_token *tokens);
int			has_heredoc(t_command *cmd);

/* Check empty */
int			has_literal_quote(t_word_part *parts);
int			should_keep_expanded_arg(char *arg, int is_from_literal);
int			has_valid_redirect_target(t_token_type type, char *target);

/* Error handling */
void		syntax_error(char *msg, t_token *token);
char		*get_token_type_name(t_token_type type);
void		parse_cleanup(t_ast_node *partial_tree, t_token *remaining_tokens);
void		parse_error_node(char *msg, t_ast_node *node);
void		parse_error_command(char *msg, t_command *cmd);
void		empty_command_error(t_command *cmd);
int			ambiguous_redirect_error(char *target);
int			heredoc_limiter_error(char *delimiter);

/* Syntax helper checks */
int			is_invalid_start_token(t_token *token);
int			is_invalid_end_token(t_token *token);
t_token		*get_last_token(t_token *tokens);
int			redirect_has_valid_target(t_token *redirect);
int			is_invalid_redirect_sequence(t_token *current);
int			has_orphan_operator_start(t_token *tokens);
int			has_orphan_operator_end(t_token *tokens);
int			has_consecutive_operators(t_token *tokens);
int			has_redirect_without_target(t_token *tokens);
int			has_redirect_before_operator(t_token *tokens);
void		syntax_error(char *message, t_token *token);

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