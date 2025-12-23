/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsarraci <lsarraci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/17 19:04:44 by lsarraci          #+#    #+#             */
/*   Updated: 2025/12/23 18:35:58 by lsarraci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DEBUG_H
# define DEBUG_H

# include "structs.h"

/* Lexer debug */
void	print_tokens(t_token *tokens);

/* Parser debug */
void	print_indent(int indent);
void	print_redirects(t_command *cmd);
void	print_command(t_command *cmd, int indent);
void	print_args(char **args);
void	print_ast(t_ast_node *tree);

#endif