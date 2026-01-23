/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsarraci <lsarraci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 13:51:29 by lsarraci          #+#    #+#             */
/*   Updated: 2026/01/23 17:07:08 by lsarraci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/shell.h"

void print_numeric(char *cmd)
{
	ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
	ft_putstr_fd(cmd, STDERR_FILENO);
	ft_putstr_fd(": numeric argument required\n", STDERR_FILENO);
}

int	builtin_exit(char **args, t_env *env)
{
	int	exit_code;
	int	arg_count;

	exit_code = env->last_exit_status;
	arg_count = count_args(args);
	ft_printf("exit\n");
	if (arg_count == 1)
	{
		env->should_exit = 1;
		env->exit_code = exit_code % 256;
		return (0);
	}
	if (!is_numeric(args[1]))
	{
		print_numeric(args[1]);
		env->should_exit = 1;
		env->exit_code = 2;
		return (0);
	}
	if (arg_count > 2)
	{
		ft_putstr_fd("minishell: too many arguments\n", STDERR_FILENO);
		return (0);
	}
	exit_code = ft_atoi(args[1]);
	env->should_exit = 1;
	env->exit_code = ((unsigned char)exit_code);
	return (0);
}
