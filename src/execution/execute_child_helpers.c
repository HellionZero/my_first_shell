/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_child_helpers.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsarraci <lsarraci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/29 16:04:35 by lsarraci          #+#    #+#             */
/*   Updated: 2026/01/29 16:04:39 by lsarraci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/shell.h"

static void	handle_exec_error_local(char *cmd)
{
	if (errno == EPIPE)
	{
		ft_putstr_fd(": Broken pipe\n", STDERR_FILENO);
		return ;
	}
	if (errno == ENOEXEC || errno == ENOENT)
	{
		if (!ft_strchr(cmd, '/'))
		{
			ft_putstr_fd(cmd, STDERR_FILENO);
			ft_putstr_fd(": command not found\n", STDERR_FILENO);
		}
		else
		{
			ft_putstr_fd(cmd, STDERR_FILENO);
			ft_putstr_fd(": No such file or directory\n", STDERR_FILENO);
		}
		return ;
	}
	perror(cmd);
}

void	child_exec_external(t_command *cmd, t_env *env)
{
	char	*executable;
	char	**envp;

	if (!apply_redirects(cmd))
		child_cleanup_and_exit(env, 1);
	executable = find_executable(cmd->args[0], env);
	envp = env_to_array(env);
	if (!envp)
		child_cleanup_and_exit(env, 1);
	if (executable)
		execve(executable, cmd->args, envp);
	else
		execve(cmd->args[0], cmd->args, envp);
	handle_exec_error_local(cmd->args[0]);
	free_env_array(envp);
	if (executable)
		free(executable);
	child_cleanup_and_exit(env, 127);
}
