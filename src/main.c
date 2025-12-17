/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsarraci <lsarraci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/09 16:22:34 by lsarraci          #+#    #+#             */
/*   Updated: 2025/12/17 19:23:41 by lsarraci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/shell.h"

int	main(void)
{
	char	*input;
	t_token	*tokens;
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
			tokens = lexer(input);
			if (!tokens)
			{
				ft_printf("Error: lexer failed");
				free(input);
				continue ;
			}
			print_tokens(tokens);
			status = parser(input);
			token_list_free(tokens);
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
