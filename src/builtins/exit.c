/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsarraci <lsarraci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 13:51:29 by lsarraci          #+#    #+#             */
/*   Updated: 2026/01/23 19:08:08 by lsarraci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/shell.h"

static int	is_numeric_arg(char *str)
{
	int	i;

	i = 0;
	if (!str)
		return (0);
	if (str[0] == '-' || str[0] == '+')
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

static int	return_exit_code(char **str)
{
	int	code;

	code = 0;
	if (!str || !str[1])
		code = 0;
	else if (is_numeric_arg(str[1]))
		code = ft_atoi(str[1]);
	else
		code = 255;
	return (code % 256);
}

static void	return_numeric_error(char *arg)
{
	ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
	ft_putstr_fd(arg, STDERR_FILENO);
	ft_putstr_fd(": numeric argument required\n", STDERR_FILENO);
}

int	builtin_exit(char **args, t_env *env)
{
	int	arg_count;
	int	exit_code;

	arg_count = count_args(args);
	exit_code = 0;
	ft_printf("exit\n");
	if (arg_count > 2)
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", STDERR_FILENO);
		env->should_exit = 0;
		env->exit_code = 1;
		return (1);
	}
	if (arg_count > 1 && !is_numeric_arg(args[1]))
	{
		return_numeric_error(args[1]);
		env->should_exit = 1;
		env->exit_code = 255;
		return (255);
	}
	exit_code = return_exit_code(args);
	env->should_exit = 1;
	env->exit_code = exit_code;
	ft_printf("terminated with exit code: %d\n", exit_code);
	return (exit_code);
}
