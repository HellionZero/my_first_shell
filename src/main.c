/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsarraci <lsarraci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/09 16:22:34 by lsarraci          #+#    #+#             */
/*   Updated: 2025/12/16 18:55:45 by lsarraci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/shell.h"

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
			status = parser(input);
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
