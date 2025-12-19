/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsarraci <lsarraci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/17 17:30:57 by lsarraci          #+#    #+#             */
/*   Updated: 2025/12/19 17:32:49 by lsarraci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/shell.h"

int	is_operator(char c)
{
	return (c == '|' || c == '<' || c == '>' || c == '&');
}

int	is_not_special(char c)
{
	return (c != '\'' && c != '"' && c != '$');
}

int	is_word_char(char c)
{
	return (!is_operator(c) && c != ' ' && c != '\t'
		&& is_not_special(c));
}

int	skip_spaces(char *input, int i)
{
	while (input[i] && (input[i] == ' ' || input[i] == '\t'))
		i++;
	return (i);
}

int	is_word_boundary(char c)
{
	return (c == ' ' || c == '\t' || is_operator(c) || c == '\0');
}
