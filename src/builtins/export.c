/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: loda-sil <loda-sil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/02 18:43:44 by lsarraci          #+#    #+#             */
/*   Updated: 2026/01/11 22:39:43 by loda-sil         ###   ########.fr       */
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

static int	valid_identifier(char **key, char **arg)
{
	if (is_valid_identifier(*key))
		return (1);
	ft_printf("minishell: export: `%s': not a valid identifier\n", *arg);
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
	if (!valid_identifier(&key, &arg))
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

int	builtin_export(char **args, t_env *env)
{
	int	i;

	if (!env)
		return (1);
	if (!args[1])
	{
		print_sorted_env(env);
		return (0);
	}
	i = 1;
	while (args[i])
	{
		if (process_export_arg(args[i], env) != 0)
			return (1);
		i++;
	}
	return (0);
}
