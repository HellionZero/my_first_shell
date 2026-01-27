/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_init.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsarraci <lsarraci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/06 19:39:01 by lsarraci          #+#    #+#             */
/*   Updated: 2026/01/27 15:16:31 by lsarraci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/shell.h"

t_env	*init_shell(char **envp)
{
	t_env	*env;

	env = init_env(envp);
	if (!env)
		return (NULL);
	if (isatty(STDIN_FILENO))
		display_banner();
	return (env);
}

void	cleanup_shell(t_env *env)
{
	if (!env)
		return ;
	free_env(env);
}

void	shell_cleanup_and_exit(t_env *env, int exit_code)
{
	rl_clear_history();
	cleanup_shell(env);
	exit(exit_code);
}
