/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsarraci <lsarraci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 15:30:00 by lsarraci          #+#    #+#             */
/*   Updated: 2025/12/19 14:25:44 by lsarraci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/shell.h"

t_token	*token_new(t_token_type type, char *value, int pos)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->type = type;
	token->value = ft_strdup(value);
	if (!token->value)
	{
		free(token);
		return (NULL);
	}
	token->pos = pos;
	token->length = ft_strlen(value);
	token->quote_type = 0;
	token->next = NULL;
	return (token);
}

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

void	token_free(t_token *token)
{
	if (!token)
		return ;
	if (token->value)
		free(token->value);
	free(token);
}

void	token_list_free(t_token *tokens)
{
	t_token	*tmp;

	while (tokens)
	{
		tmp = tokens->next;
		token_free(tokens);
		tokens = tmp;
	}
}

void	token_add_back(t_token **list, t_token *new)
{
	t_token	*current;

	if (!list || !new)
		return ;
	if (*list == NULL)
	{
		*list = new;
		return ;
	}
	current = *list;
	while (current->next)
		current = current->next;
	current->next = new;
}
