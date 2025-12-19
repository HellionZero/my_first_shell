/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_debug.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsarraci <lsarraci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/17 18:54:49 by lsarraci          #+#    #+#             */
/*   Updated: 2025/12/19 18:47:17 by lsarraci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/shell.h"

static const char	**get_part_type_names(void)
{
	static const char	*names[] = {
		"LITERAL",
		"SINGLE_QUOTE",
		"DOUBLE_QUOTE",
		"VARIABLE"
	};

	return (names);
}

static const char	**get_token_type_names(void)
{
	static const char	*names[] = {
		"WORD",
		"PIPE",
		"REDIR_IN",
		"REDIR_OUT",
		"APPEND",
		"HEREDOC",
		"SINGLE_QUOTE",
		"DOUBLE_QUOTE",
		"VARIABLE",
		"AND",
		"OR"
	};

	return (names);
}

static void	print_word_parts(t_word_part *parts)
{
	t_word_part		*current;
	const char		**part_type_names;

	part_type_names = get_part_type_names();
	current = parts;
	ft_printf("  Parts: ");
	while (current)
	{
		ft_printf("[%s:'%s']", part_type_names[current->type],
			current->content);
		if (current->next)
			ft_printf(" + ");
		current = current->next;
	}
	ft_printf("\n");
}

void	print_tokens(t_token *tokens)
{
	t_token			*current;
	int				i;
	const char		**type_names;

	type_names = get_token_type_names();
	current = tokens;
	i = 0;
	ft_printf("\n=== TOKENS ===\n");
	while (current)
	{
		ft_printf("[%d] %s", current->pos, type_names[current->type]);
		if (current->type == TOKEN_WORD && current->parts)
		{
			ft_printf("\n");
			print_word_parts(current->parts);
		}
		else if (current->value)
			ft_printf(": '%s'\n", current->value);
		else
			ft_printf("\n");
		current = current->next;
		i++;
	}
	ft_printf("Total: %d tokens\n==============\n\n", i);
}
