/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsarraci <lsarraci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/02 18:43:44 by lsarraci          #+#    #+#             */
/*   Updated: 2026/01/23 15:59:34 by lsarraci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/shell.h"

static void	print_sorted_env(t_env *env)
{
	t_env_var	*current;

	current = env->vars;
	while (current)
	{
		ft_printf("declare -x %s=\"%s\"\n", current->key, current->value);
		current = current->next;
	}
}

static int	valid_identifier(char **key)
{
	if (is_valid_identifier(*key))
		return (1);
	ft_putstr_fd("minishell: not a valid identifier\n", STDERR_FILENO);
	free(*key);
	*key = NULL;
	return (0);
}

static int	process_export_arg(char *arg, t_env *env)
{
	char	*equal_sign;
	char	*key;
	char	*value;

	equal_sign = ft_strchr(arg, '=');
	if (!equal_sign)
		return (0);
	key = ft_substr(arg, 0, equal_sign - arg);
	if (!key)
		return (1);
	if (!valid_identifier(&key))
		return (1);
	value = ft_strdup(equal_sign + 1);
	if (!value)
	{
		free(key);
		return (1);
	}
	env_set(env, key, value);
	free(key);
	free(value);
	return (0);
}

static int	handle_export_arg(char *arg, t_env *env)
{
	char	*equal_sign;
	char	*key;
	int		result;

	equal_sign = ft_strchr(arg, '=');
	result = 0;
	if (equal_sign)
		key = ft_substr(arg, 0, equal_sign - arg);
	else
		key = ft_strdup(arg);
	if (!key)
		return (1);
	if (!is_valid_identifier(key))
	{
		ft_putstr_fd("minishell: not a valid identifier\n", STDERR_FILENO);
		free(key);
		return (1);
	}
	free(key);
	if (equal_sign)
		result = process_export_arg(arg, env);
	return (result);
}

int	builtin_export(char **args, t_env *env)
{
	int	i;
	int	ret;

	i = 1;
	ret = 0;
	if (!env)
		return (1);
	if (!args[1])
	{
		print_sorted_env(env);
		return (0);
	}
	while (args[i])
	{
		if (handle_export_arg(args[i], env) != 0)
			ret = 1;
		i++;
	}
	return (ret);
}
