/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsarraci <lsarraci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/09 16:22:34 by lsarraci          #+#    #+#             */
/*   Updated: 2025/12/16 14:51:47 by lsarraci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/shell.h"

int	parser(char *input)
{
	char	**args;
	int		status;

	args = ft_split(input, ' ');
	if (!args)
		return (-1);
	if (ft_strncmp(args[0], "exit", 5) == 0)
	{
		status = builtin_exit(args);
		ft_free_split(args);
		return (status);
	}
	else
		ft_printf("command not found: %s\n", args[0]);
	ft_free_split(args);
	return (-1);
}
