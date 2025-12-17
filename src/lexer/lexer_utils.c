/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsarraci <lsarraci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/17 17:30:57 by lsarraci          #+#    #+#             */
/*   Updated: 2025/12/17 19:22:10 by lsarraci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/shell.h"

t_token	*create_single_token(char *op_string,
			t_token_type type, int *i, int pos)
{
	char	*value;

	value = ft_strdup(op_string);
	if (!value)
		return (NULL);
	(*i) += ft_strlen(op_string);
	return (token_new(type, value, pos));
}

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
