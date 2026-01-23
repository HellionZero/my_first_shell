/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsarraci <lsarraci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 13:48:12 by lsarraci          #+#    #+#             */
/*   Updated: 2026/01/23 15:24:31 by lsarraci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/shell.h"

static char	*get_target_dir(char **args, t_env *env)
{
	char	*target;

	if (!args[1])
	{
		target = get_env_value(env, "HOME");
		if (!target)
		{
			ft_printf("minishell: cd: HOME not set\n");
			return (NULL);
		}
		return (target);
	}
	if (ft_strcmp(args[1], "-") == 0)
	{
		target = get_env_value(env, "OLDPWD");
		if (!target)
		{
			ft_printf("minishell: cd: OLDPWD not set\n");
			return (NULL);
		}
		ft_printf("%s\n", target);
		return (target);
	}
	return (args[1]);
}

static int	change_directory(char *path, t_env *env)
{
	char	old_pwd[PATH_MAX];
	char	new_pwd[PATH_MAX];

	if (!getcwd(old_pwd, PATH_MAX))
	{
		perror("cd: getcwd");
		return (1);
	}
	if (chdir(path) != 0)
	{
		perror("cd");
		return (1);
	}
	if (!getcwd(new_pwd, PATH_MAX))
	{
		perror("cd: getcwd");
		return (1);
	}
	env_set(env, "OLDPWD", old_pwd);
	env_set(env, "PWD", new_pwd);
	return (0);
}

int	builtin_cd(char **args, t_env *env)
{
	char	*target;
	int		arg_count;

	if (!env)
		return (1);
	arg_count = count_args(args);
	if (arg_count > 2)
	{
		ft_putstr_fd("minishell: too many arguments", STDERR_FILENO);
		return (1);
	}
	target = get_target_dir(args, env);
	if (!target)
		return (1);
	return (change_directory(target, env));
}
