/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_child_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsarraci <lsarraci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/19 15:25:14 by lsarraci          #+#    #+#             */
/*   Updated: 2026/01/19 15:26:13 by lsarraci         ###   ########.fr       */
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
	if (!cmd || !cmd->args || !cmd->args[0])
		child_cleanup_and_exit(env, 0);
	if (!apply_redirects(cmd))
		child_cleanup_and_exit(env, 1);
	exit_code = execute_builtin(cmd->args, env);
	child_cleanup_and_exit(env, exit_code);
}

void	child_execute_command(t_command *cmd, t_env *env)
{
	char	*executable;
	char	**envp;

	restore_signals_default();
	if (!cmd || !cmd->args || !cmd->args[0])
		child_cleanup_and_exit(env, 0);
	if (!apply_redirects(cmd))
		child_cleanup_and_exit(env, 1);
	executable = find_executable(cmd->args[0], env);
	if (!executable)
	{
		ft_printf("%s: command not found\n", cmd->args[0]);
		child_cleanup_and_exit(env, 127);
	}
	envp = env_to_array(env);
	if (!envp)
	{
		free(executable);
		child_cleanup_and_exit(env, 1);
	}
	execve(executable, cmd->args, envp);
	ft_printf("%s: execution failed\n", cmd->args[0]);
	free_env_array(envp);
	free(executable);
	child_cleanup_and_exit(env, 126);
}
