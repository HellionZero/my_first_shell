/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsarraci <lsarraci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/17 17:30:57 by lsarraci          #+#    #+#             */
/*   Updated: 2025/12/19 14:25:15 by lsarraci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/shell.h"

int	is_operator(char c)
{
	return (c == '|' || c == '<' || c == '>' || c == '&');
}

int	is_word_char(char c)
{
	return (!is_operator(c) && c != ' ' && c != '\t'
		&& c != '\'' && c != '"' && c != '$');
}

int	skip_spaces(char *input, int i)
{
	while (input[i] && (input[i] == ' ' || input[i] == '\t'))
		i++;
	return (i);
}
/*char	*extract_quoted_word(char *input, int *i)
{
	
}

char	*extract_variable(char *input, int *i)
{
	
}*/
