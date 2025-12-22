/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_status.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsarraci <lsarraci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/22 19:16:24 by lsarraci          #+#    #+#             */
/*   Updated: 2025/12/22 19:16:25 by lsarraci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/shell.h"

static int	*exit_status_ptr(void)
{
	static int	status = 0;

	return (&status);
}

void	set_exit_status(int status)
{
	int	*exit_status;

	exit_status = exit_status_ptr();
	if (status < 0)
		*exit_status = 1;
	else if (status > 255)
		*exit_status = status % 256;
	else
		*exit_status = status;
}

int	get_exit_status(void)
{
	return (*exit_status_ptr());
}
