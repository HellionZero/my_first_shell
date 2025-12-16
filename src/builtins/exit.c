/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsarraci <lsarraci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 13:51:29 by lsarraci          #+#    #+#             */
/*   Updated: 2025/12/16 14:49:27 by lsarraci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/shell.h"

int	builtin_exit(char **args)
{
	int	code;

	ft_printf("exit\n");
	code = 0;
	if (args && args[1])
		code = ft_atoi(args[1]);
	return (code);
}

