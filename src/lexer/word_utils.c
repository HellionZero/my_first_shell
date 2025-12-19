/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   word_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsarraci <lsarraci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/19 15:11:06 by lsarraci          #+#    #+#             */
/*   Updated: 2025/12/19 17:07:54 by lsarraci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/shell.h"

t_word_part	*word_part_new(t_word_part_type type, char *content)
{
	t_word_part		*part;

	part = malloc(sizeof(t_word_part));
	if (!part)
		return (NULL);
	part->type = type;
	part->content = content;
	part->next = NULL;
	return (part);
}

void	word_part_free(t_word_part	*part)
{
	if (!part)
		return ;
	if (part->content)
		free(part->content);
	free(part);
}

void	word_part_list_free(t_word_part	*parts)
{
	t_word_part	*tmp;

	while (parts)
	{
		tmp = parts->next;
		word_part_free(parts);
		parts = tmp;
	}
}

void	word_part_add_back(t_word_part **list, t_word_part *new)
{
	t_word_part	*current;

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

t_token	*token_new_word(t_word_part *parts, int pos)
{
	t_token		*word_token;

	word_token = malloc(sizeof(t_token));
	if (!word_token)
		return (NULL);
	word_token->type = TOKEN_WORD;
	word_token->parts = parts;
	word_token->value = NULL;
	word_token->pos = pos;
	word_token->next = NULL;
	return (word_token);
}
