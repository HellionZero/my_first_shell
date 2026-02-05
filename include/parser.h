/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsarraci <lsarraci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 15:11:15 by lsarraci          #+#    #+#             */
/*   Updated: 2026/02/05 16:48:28 by lsarraci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include "structs.h"

/* -------- Command creation and manipulation -------- */

/* verify if the command given is valid*/
int			is_valid_command(t_command *cmd);

/* create a new command structure. */
t_command	*command_new(void);

/* free a command structure and its associated resources */
void		command_free(t_command *cmd);

/* free a list of command structures */
void		command_list_free(t_command *cmds);

/* add a command to the end of a command list
if there's no list, create a new one */
void		command_list_add_back(t_command **list, t_command *new);

/* add an argument to the command's argument list.
a command arg is a string representing a single argument */
int			command_add_arg(t_command *cmd, char *arg);

/* ----------------------redirect utils---------------------- */

/* create a new redirect structure */
t_redirect	*redirect_new(t_token_type type, char *file, char *delimiter);

/* free a redirect structure */
void		redirect_free(t_redirect *redir);

/* free a list of redirect structures */
void		redirect_list_free(t_redirect *redirects);

/* add a redirect to the end of a redirect list.
if there's no list, create a new one */
void		redirect_add_back(t_redirect **list, t_redirect *new);

/* -------------- heredoc utils --------------------------*/

/*checks if the heredoc should be expanded.
the delimiter serves as a way to identify
when to stop expanding */
int			should_expand_heredoc(char *delimiter);

/* clear heredoc delimiter */
char		*clear_heredoc_delimiter(char *delimiter);

/* extract variable name from heredoc line */
char		*extract_var_name_heredoc(char *start, int *len);

/* expand heredoc line */
char		*expand_heredoc_line(char *line);

/* write a line to a pipe.
each time a line is written,
a newline character is appended */
void		write_line_to_pipe(int fd, char *line);

/* ------------ heredoc expansion utils -------------------*/

/* function to read heredoc content. 
it uses a pipe to write the content
to ensure safe and efficient handling */
void		read_heredoc_content(int pipe_fd, char *delimiter,
				char *clean_delim);

/* function to handle heredoc line expansion.
line expansions are useful to manage variable
substitutions and other expansions within heredoc content */
char		*get_expanded_line(char *line, char *delimiter);

/* checks if the delimiter is reached in the heredoc content. */
int			is_delimiter_reached(char *line, char *clean_delim);

/* heredoc processing - function to handle all 
heredoc processing. */
int			process_heredoc(char *delimiter);

/* ------------------ heredoc file utils ------------------*/

/* function to initialize heredoc file descriptors */
void		init_heredoc_fd(t_redirect *redir);

/* function to set up heredoc and manage file descriptors */
int			setup_heredoc(t_redirect *redir);

/* function to close heredoc file descriptors after use */
void		close_heredoc_fd(t_redirect *redir);

/* function to clean up heredoc file descriptors after use */
void		cleanup_heredoc_fds(t_redirect *redir);

/* function to process all heredocs in a command.
it iterates through the list of redirects in a command,
identifies heredoc redirects, and processes them accordingly. */
int			process_all_heredocs(t_command *cmd);

/* ----------------- environment variable settings -----------------*/

/* singleton accessors for shell environment. It
reads the state of shell environment, initialized
by zero, and updated on each change */
void		set_shell_env(t_env	*env);

/* retrieves the current shell environment instance. 
while the terminal is active, the values of shell 
environment variables remain consistent */
t_env		*get_shell_env(void);

/* resets the shell environment instance to NULL */
void		reset_shell_env(void);

/* ----------------------Syntax validation ----------------------*/

/* function to validate the syntax of a list of tokens */
int			validate_syntax(t_token *tokens);

/* function to check for orphan operators at the start
and end of the token list. 
if orphan operators are found, it returns a non-zero value */
int			has_orphan_operator_start(t_token *tokens);

/* function to check for orphan operators
 at the end of the token list. 
if orphan operators are found, it returns a non-zero value */
int			has_orphan_operator_end(t_token *tokens);

/* function to check for consecutive operators in the token list. it's 
used to identify syntax errors related to operator placement */
int			has_consecutive_operators(t_token *tokens);

/* checks if a redirect is present inside the list of tokens without a target.
it's used to stop the operation if such a redirect is found */
int			has_redirect_without_target(t_token *tokens);

/* checks if a redirect is present before an operator in the token list. it's a 
measure to ensure redirects are properly placed, avoiding syntax errors */
int			has_redirect_before_operator(t_token *tokens);

/* Check if heredoc is present in a command. the main use case is to prevent 
the program to behave odd when a heredoc is found. */
int			has_heredoc(t_command *cmd);

/* -------------------------- Check empty ---------------------------------*/

/* function to check if the parts inside a word
contain literal quotes. the main use case is
to prevent spell errors and variable misbehaving. */
int			has_literal_quote(t_word_part *parts);

/* function to verify if the arg of
a token should be kept after expansion */
int			should_keep_expanded_arg(char *arg, int is_from_literal);

/* function to check if a redirect target is valid.
it cannot be empty or NULL */
int			has_valid_redirect_target(t_token_type type, char *target);

/* -------------------- Error handling -------------------- */

/* Syntax error handling. verify syntax errors
and report them */
void		syntax_error(char *msg, t_token *token);

/* function to clean up parsing resources.
it frees allocated memory of the partial tree
and remaining tokens */
void		parse_cleanup(t_ast_node *partial_tree, t_token *remaining_tokens);

/* Parsing error reporting function.
Takes a message and an AST node. 
frees the ast node and print a error message to the user. */
void		parse_error_node(char *msg, t_ast_node *node);

/* Parsing error reporting function.
Takes a message and a command node. 
Different from the parse_error_node function,
the parse error command function serves to free
the command and return a message to the user.*/
void		parse_error_command(char *msg, t_command *cmd);

/* Function to handle empty command errors.
It frees the command structure and sets the exit status to 0. */
void		empty_command_error(t_command *cmd);

/* check if there are weird behavior in the redirect command, 
for example, ambiguous redirects or heredoc limiter errors */
int			ambiguous_redirect_error(char *target);

/* check if there are errors related to heredoc limiter.
it checks if the heredoc is properly terminated. */
int			heredoc_limiter_error(char *delimiter);

/* ---------------Syntax helper checks ----------------*/

/* function to return the type of a token as a string */
char		*get_token_type_name(t_token_type type);

/* verify if a token is invalid at the start of a command, 
for example, if it is an operator or <. */
int			is_invalid_start_token(t_token *token);

/* checks if a token is invalid at the end of a command, 
for example, if it is an operator or <. */
int			is_invalid_end_token(t_token *token);

/* retrieves the last token in a linked list of tokens */
t_token		*get_last_token(t_token *tokens);

/* checks if a redirect token has a valid target following it.
for example, it can't be empty or NULL */
int			redirect_has_valid_target(t_token *redirect);

/* checks for invalid redirect sequences in the token list. 
for example, if there are consecutive redirects 
without proper targets */
int			is_invalid_redirect_sequence(t_token *current);

/* checks if there is a redirect before
an operator in the token list. 
It's a measure to ensure redirects are
properly placed, avoiding syntax errors */
int			has_redirect_before_operator(t_token *tokens);

/* checks if there is a redirect without
a target in the token list.
if there is, the function returns a non-zero value. */
int			has_redirect_without_target(t_token *tokens);

/* ------------------ Operator functions ------------------*/
int			has_orphan_operator_start(t_token *tokens);
int			has_orphan_operator_end(t_token *tokens);
int			has_consecutive_operators(t_token *tokens);

/* ----- AST node creation and manipulation -------*/

/* create a new AST node. */
t_ast_node	*node_new(void);

/* create a new command node. */
t_ast_node	*node_new_command(t_command *cmd);

/* create a new operator node with left and right children. */
t_ast_node	*node_new_operator(t_node_type type, t_ast_node *left,
				t_ast_node *right);

/* free an AST node and its children recursively. */
void		node_free(t_ast_node *node);

/*  -----General utility functions 
(used across multiple modules) ----- */

/* function to join two strings and free the originals */
char		*join_and_free(char *s1, char *s2);

/* function to join a string and a character,
freeing the original string.
the main use case is to append a single
character to a string, like 'a' or a equal sign.
*/
char		*join_char_and_free(char *s, char c);

/* check if the character is a quote */
int			has_quotes(char c);

/* check if the character is valid for variable names */
int			is_var_char(char c);

/* check if a string needs variable expansion */
int			needs_expansion(char *str);

/* ----------- Expansion functions ----------- */

/* function to expand a variable. */
char		*expand_variable(char *var_name);

/* function to expand a word composed of multiple parts. */
char		*expand_word(t_word_part *parts);

/* ----------- special variables ----------- */

/* check if a variable name is a special variable
for example $? or $$. */
int			is_special_var(char *var_name);

/* expand a special variable */
char		*expand_special_var(char *var_name);

/* ------------------ parser utils ------------------ */

/* check if a token is a word or a redirect,
to ensure proper parsing */
int			is_word_or_redirect(t_token *token);

/* check if a token is an operator */
int			is_operator_token(t_token *token);

/* check if a token is a redirect */
int			is_redirect(t_token *token);

/* function to handle a redirect token */
int			handle_redirect(t_command *cmd, t_token **tokens);

/* --------------- parser main functions ---------------- */

/* function to parse logical expressions */
t_ast_node	*parse_logical(t_token **tokens);

/* function to parse pipelines */
t_ast_node	*parse_pipeline(t_token **tokens);

/* function to parse simple commands */
t_ast_node	*parse_simple_cmd(t_token **tokens);

/* MAIN FUNCTION: parse the tokens and build an AST tree.*/
t_ast_node	*parse_tokens(t_token *tokens);

#endif