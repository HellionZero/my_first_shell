/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsarraci <lsarraci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/09 16:22:34 by lsarraci          #+#    #+#             */
/*   Updated: 2025/12/23 19:03:26 by lsarraci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/shell.h"

static void	process_input_debug(char *input)
{
	t_token		*tokens;
	t_ast_node	*tree;

	tokens = lexer(input);
	if (!tokens)
	{
		ft_printf("[DEBUG] Lexer failed\n");
		return ;
	}
	print_tokens(tokens);
	tree = parse_tokens(tokens);
	if (!tree)
	{
		ft_printf("[DEBUG] Parser failed\n");
		token_list_free(tokens);
		return ;
	}
	print_ast(tree);
	node_free(tree);
	token_list_free(tokens);
}

int	main(void)
{
	char	*input;

	display_banner();
	setup_signals_interactive();
	while (1)
	{
		input = readline(get_colored_prompt());
		if (!input)
			break ;
		if (input[0])
		{
			add_history(input);
			process_input_debug(input);
		}
		free(input);
	}
	return (0);
}
