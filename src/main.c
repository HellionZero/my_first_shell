/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsarraci <lsarraci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/09 16:22:34 by lsarraci          #+#    #+#             */
/*   Updated: 2025/12/22 19:15:59 by lsarraci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/shell.h"

static int	process_input(char *input)
{
	t_token		*tokens;
	t_ast_node	*tree;
	int			status;

	tokens = lexer(input);
	if (!tokens)
	{
		ft_printf("Error: lexer failed\n");
		return (-1);
	}
	print_tokens(tokens);
	tree = parse_tokens(tokens);
	if (!tree)
	{
		ft_printf("Error: parser failed\n");
		token_list_free(tokens);
		return (-1);
	}
	print_ast(tree);
	status = parser(input);
	node_free(tree);
	token_list_free(tokens);
	return (status);
}

int	main(void)
{
	char	*input;
	int		status;

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
			status = process_input(input);
			if (status >= 0)
			{
				free(input);
				rl_clear_history();
				return (status);
			}
		}
		free(input);
	}
	return (0);
}
