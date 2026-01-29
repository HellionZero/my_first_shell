/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_child_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsarraci <lsarraci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/19 15:25:14 by lsarraci          #+#    #+#             */
/*   Updated: 2026/01/29 16:06:39 by lsarraci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/shell.h"

void	child_cleanup_and_exit(t_env *env, int exit_code)
{
	if (env)
		free_env(env);
	exit(exit_code);
}

void	child_execute_builtin(t_command *cmd, t_env *env)
{
	int	exit_code;

	restore_signals_default();
	if (is_pure_redirect_command(cmd))
	{
		ensure_redirect_files_created(cmd);
		child_cleanup_and_exit(env, 0);
	}
	if (cmd->args && cmd->args[0] && cmd->args[0][0] != '\0')
	{
		if (!apply_redirects(cmd))
			child_cleanup_and_exit(env, 1);
		exit_code = execute_builtin(cmd->args, env);
		child_cleanup_and_exit(env, exit_code);
	}
	else
	{
		ensure_redirect_files_created(cmd);
		child_cleanup_and_exit(env, 0);
	}
}

void	child_execute_command(t_command *cmd, t_env *env)
{
	restore_signals_default();
	if (is_pure_redirect_command(cmd))
	{
		ensure_redirect_files_created(cmd);
		child_cleanup_and_exit(env, 0);
	}
	if (cmd->args && cmd->args[0] && cmd->args[0][0] != '\0')
		child_exec_external(cmd, env);
	else
	{
		ensure_redirect_files_created(cmd);
		child_cleanup_and_exit(env, 0);
	}
}
