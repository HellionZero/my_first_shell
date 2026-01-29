/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_stdio.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsarraci <lsarraci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/29 15:49:43 by lsarraci          #+#    #+#             */
/*   Updated: 2026/01/29 15:49:50 by lsarraci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/shell.h"

int	save_stdio(int *saved_stdin, int *saved_stdout)
{
	*saved_stdin = dup(0);
	if (*saved_stdin < 0)
		return (0);
	*saved_stdout = dup(1);
	if (*saved_stdout < 0)
	{
		close(*saved_stdin);
		return (0);
	}
	return (1);
}
