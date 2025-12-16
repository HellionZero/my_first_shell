/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsarraci <lsarraci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 15:04:24 by lsarraci          #+#    #+#             */
/*   Updated: 2025/12/16 15:05:01 by lsarraci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
# define LEXER_H

# include "structs.h"

/* Token creation and manipulation */
t_token	*token_new(t_token_type type, char *value, int pos);
void	token_free(t_token *token);
void	token_list_free(t_token *tokens);
void	token_list_add_back(t_token **list, t_token *new);

#endif