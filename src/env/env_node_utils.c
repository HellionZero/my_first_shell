/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_node_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsarraci <lsarraci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/02 16:32:45 by lsarraci          #+#    #+#             */
/*   Updated: 2026/01/02 18:12:24 by lsarraci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/shell.h"

t_env_var	*create_env_node(char *key, char *value)
{
	t_env_var	*node;

	node = malloc(sizeof(t_env_var));
	if (!node)
		return (NULL);
	node->key = ft_strdup(key);
	if (!node->key)
	{
		free(node);
		return (NULL);
	}
	node->value = ft_strdup(value);
	if (!node->value)
	{
		free(node->key);
		free(node);
		return (NULL);
	}
	node->next = NULL;
	return (node);
}

void	free_env_node(t_env_var *node)
{
	if (!node)
		return ;
	free(node->key);
	free(node->value);
	free(node);
}

void	free_env_list(t_env_var *list)
{
	t_env_var	*current;
	t_env_var	*next;

	current = list;
	while (current)
	{
		next = current->next;
		free_env_node(current);
		current = next;
	}
}

void	free_env(t_env *env)
{
	if (!env)
		return ;
	free_env_list(env->vars);
	free(env);
}
