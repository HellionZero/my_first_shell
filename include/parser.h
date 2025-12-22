/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsarraci <lsarraci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 15:11:15 by lsarraci          #+#    #+#             */
/*   Updated: 2025/12/22 18:01:34 by lsarraci         ###   ########.fr       */
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

/* AST node creation and manipulation */
t_ast_node	*node_new(void);
t_ast_node	*node_new_command(t_command *cmd);
t_ast_node	*node_new_operator(t_node_type type, t_ast_node *left,
				t_ast_node *right);
void		node_free(t_ast_node *node);

/*expand functions*/
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