/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_builtin_files.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsarraci <lsarraci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/09 19:28:30 by lsarraci          #+#    #+#             */
/*   Updated: 2026/01/09 19:28:40 by lsarraci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/shell.h"

int	execute_builtin_files(char **args, t_env *env)
{
	if (!args || !args[0])
		return (1);
	if (ft_strcmp(args[0], "pwd") == 0)
		return (builtin_pwd(args, env));
	else if (ft_strcmp(args[0], "cd") == 0)
		return (builtin_cd(args, env));
	return (1);
}
