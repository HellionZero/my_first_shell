/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsarraci <lsarraci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/02 15:31:38 by lsarraci          #+#    #+#             */
/*   Updated: 2026/01/02 17:55:13 by lsarraci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/shell.h"

t_env_var	*env_get(t_env *env, char *key)
{
	t_env_var	*current;

	if (env == NULL || key == NULL)
		return (NULL);
	current = env->vars;
	while (current != NULL)
	{
		if (ft_strcmp(current->key, key) == 0)
			return (current);
		current = current->next;
	}
	return (NULL);
}

void	env_set(t_env *env, char *key, char *value)
{
	t_env_var	*var;
	t_env_var	*node;
	char		*new_value;

	if (!env || !key || !value)
		return ;
	var = env_get(env, key);
	if (var)
	{
		new_value = ft_strdup(value);
		if (!new_value)
			return ;
		free(var->value);
		var->value = new_value;
		return ;
	}
	node = create_env_node(key, value);
	if (!node)
		return ;
	node->next = env->vars;
	env->vars = node;
}

void	env_unset(t_env *env, char *key)
{
	t_env_var	*current;
	t_env_var	*prev;

	if (!env || !key || !env->vars)
		return ;
	if (ft_strcmp(env->vars->key, key) == 0)
	{
		current = env->vars;
		env->vars = env->vars->next;
		free_env_node(current);
		return ;
	}
	prev = env->vars;
	current = env->vars->next;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
		{
			prev->next = current->next;
			free_env_node(current);
			return ;
		}
		prev = current;
		current = current->next;
	}
}

char	*get_env_value(t_env *env, char *key)
{
	t_env_var	*var;

	var = env_get(env, key);
	if (var)
		return (var->value);
	return (NULL);
}
