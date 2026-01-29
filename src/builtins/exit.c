/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsarraci <lsarraci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 13:51:29 by lsarraci          #+#    #+#             */
/*   Updated: 2026/01/26 18:24:13 by lsarraci         ###   ########.fr       */
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

static int	return_exit_code(char **str, t_env *env)
{
	if (!str || !str[1])
		return (env->exit_code);
	else if (is_numeric_arg(str[1]))
		return ((unsigned char)ft_atoi(str[1]) % 256);
	else
		return (255);
}

static void	return_numeric_error(char *arg)
{
	ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
	ft_putstr_fd(arg, STDERR_FILENO);
	ft_putstr_fd(": numeric argument required\n", STDERR_FILENO);
}

static int	check_args(int argc, char **args, t_env *env)
{
	if (argc > 2)
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", STDERR_FILENO);
		env->should_exit = 0;
		env->exit_code = 1;
		set_exit_status(1);
		return (1);
	}
	if (argc > 1 && !is_numeric_arg(args[1]))
	{
		if (args[1])
			return_numeric_error(args[1]);
		env->should_exit = 1;
		env->exit_code = 2;
		set_exit_status(2);
		return (2);
	}
	return (0);
}

int	builtin_exit(char **args, t_env *env)
{
	int	arg_count;
	int	exit_code;

	arg_count = count_args(args);
	exit_code = 0;
	ft_printf("exit\n");
	if (check_args(arg_count, args, env))
		return (env->exit_code);
	exit_code = return_exit_code(args, env);
	env->should_exit = 1;
	env->exit_code = exit_code;
	set_exit_status(exit_code);
	return (exit_code);
}
