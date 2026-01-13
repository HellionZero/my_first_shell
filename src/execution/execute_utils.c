/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsarraci <lsarraci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/02 15:16:54 by lsarraci          #+#    #+#             */
/*   Updated: 2026/01/10 14:31:35 by lsarraci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/shell.h"

int	is_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	if ((ft_strcmp(cmd, "echo") == 0) || (ft_strcmp(cmd, "cd") == 0))
		return (1);
	if ((ft_strcmp(cmd, "pwd") == 0) || (ft_strcmp(cmd, "export") == 0))
		return (1);
	if ((ft_strcmp(cmd, "unset") == 0) || (ft_strcmp(cmd, "env") == 0))
		return (1);
	if (ft_strcmp(cmd, "exit") == 0)
		return (1);
	return (0);
}

int	is_valid_identifier(char *str)
{
	int	i;

	if (!str || !str[0])
		return (0);
	if (!ft_isalpha(str[0]) && str[0] != '_')
		return (0);
	i = 1;
	while (str[i])
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

int	count_args(char **args)
{
	int	i;

	i = 0;
	while (args[i])
		i++;
	return (i);
}

int	is_numeric(char *str)
{
	int	i;

	if (!str || !str[0])
		return (0);
	i = 0;
	if (str[i] == '+' || str[i] == '-')
		i++;
	if (!str[i])
		return (0);
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

char	*define_executable(t_command *cmd, t_env *env)
{
	char	*executable;

	executable = find_executable(cmd->args[0], env);
	if (executable == (char *)-1)
	{
		ft_printf("%s: Permission denied\n", cmd->args[0]);
		return ((char *)-1);
	}
	if (!executable)
	{
		ft_printf("%s: command not found\n", cmd->args[0]);
		return (NULL);
	}
	return (executable);
}
