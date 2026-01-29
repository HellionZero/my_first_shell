/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_external.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsarraci <lsarraci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/10 13:45:48 by lsarraci          #+#    #+#             */
/*   Updated: 2026/01/10 13:59:06 by lsarraci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/shell.h"

int	execute_external_command(char **args, t_env *env)
{
	t_command	cmd;

	if (!args || !args[0])
		return (0);
	cmd.args = args;
	cmd.redirects = NULL;
	return (execute_command(&cmd, env));
}
