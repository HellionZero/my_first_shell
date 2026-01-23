/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsarraci <lsarraci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 13:51:29 by lsarraci          #+#    #+#             */
/*   Updated: 2026/01/23 17:58:27 by lsarraci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/shell.h"

static void	print_numeric(char *cmd, t_env *env)
{
	ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
	ft_putstr_fd(cmd, STDERR_FILENO);
	ft_putstr_fd(": numeric argument required\n", STDERR_FILENO);
	env->should_exit = 1;
	env->exit_code = 255;
}

static int	return_exit_code(char **str, t_env *env)
{
	int	code;

	if (!str || !str[1])
		code = 0;
	else if (is_numeric(str[1]))
		code = ft_atoi(str[1]);
	else
		print_numeric(str[1], env);
	ft_free_split(str);
	return (code % 256);
}

static int	check_valid_args(int argc, char **args)
{
	int	i;

	i = 0;
	while (args[1][i])
	{
		if (!is_numeric(args[1]))
		{
			print_numeric(args[1], NULL);
			return (0);
		}
		i++;
	}
	if (argc > 2)
	{
		ft_putstr_fd("minishell: too many arguments\n", STDERR_FILENO);
		return (0);
	}
	return (1);
}

int	builtin_exit(char **args, t_env *env)
{
	int	arg_count;
	int	valid;

	valid = 1;
	arg_count = count_args(args);
	ft_printf("exit\n");
	if (arg_count == 1)
	{
		env->should_exit = 1;
		env->exit_code = return_exit_code(args, env);
		return (0);
	}
	valid = check_valid_args(count_args(args), args);
	if (!valid)
		return (0);
	env->should_exit = 1;
	env->exit_code = return_exit_code(args, env);
	return (0);
}
