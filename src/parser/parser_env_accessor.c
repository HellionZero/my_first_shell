/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_env_accessor.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsarraci <lsarraci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/10 15:01:30 by lsarraci          #+#    #+#             */
/*   Updated: 2026/01/10 15:26:32 by lsarraci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/shell.h"

static t_env	**get_env_instance(void)
{
	static t_env	*env = NULL;

	return (&env);
}

void	set_shell_env(t_env	*new_env)
{
	*get_env_instance() = new_env;
}

t_env	*get_shell_env(void)
{
	return (*get_env_instance());
}

void	reset_shell_env(void)
{
	*get_env_instance() = NULL;
}
