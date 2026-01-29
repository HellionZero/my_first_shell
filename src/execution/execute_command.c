/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_command.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsarraci <lsarraci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/02 14:54:59 by lsarraci          #+#    #+#             */
/*   Updated: 2026/01/29 15:57:38 by lsarraci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/shell.h"

int	fork_and_execute(t_command *cmd, char *executable, t_env *env)
{
	pid_t	pid;
	t_env	*child_env;

	(void)executable;
	setup_signals_executing();
	pid = fork();
	if (pid == -1)
	{
		free(executable);
		perror("fork");
		return (1);
	}
	if (pid == 0)
	{
		child_env = deep_copy_env(env);
		child_execute_command(cmd, child_env);
		free_env(child_env);
	}
	free(executable);
	return (wait_child(pid));
}

int	execute_command(t_command *cmd, t_env *env)
{
	int	handled;

	if (is_pure_redirect_command(cmd))
	{
		ensure_redirect_files_created(cmd);
		return (0);
	}
	handled = empty_handle_manager(cmd, env);
	if (handled != -1)
		return (handled);
	if (cmd->args && cmd->args[0])
	{
		if (is_builtin(cmd->args[0]) && cmd->redirects)
			return (execute_redirects(cmd, -1, env));
		if (is_builtin(cmd->args[0]) && !env->in_pipeline)
			return (execute_builtin(cmd->args, env));
		return (prepare_and_spawn_external(cmd, env));
	}
	return (0);
}
