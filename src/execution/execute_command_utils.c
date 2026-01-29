/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_command_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsarraci <lsarraci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/27 18:13:42 by lsarraci          #+#    #+#             */
/*   Updated: 2026/01/29 15:58:58 by lsarraci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/shell.h"

char	*get_command_executable(t_command *cmd, t_env *env)
{
	char	*executable;

	if (ft_strchr(cmd->args[0], '/'))
	{
		executable = ft_strdup(cmd->args[0]);
		if (!executable)
			return ((char *)-1);
		if (!validate_executable(executable))
		{
			free(executable);
			executable = NULL;
		}
		else if (validate_executable(executable) == 126)
		{
			ft_putstr_fd(": Permission denied\n", STDERR_FILENO);
			free(executable);
			return ((char *)-1);
		}
	}
	else
		executable = find_executable(cmd->args[0], env);
	return (executable);
}

int	set_command_code(t_command *cmd, char **executable)
{
	if (*executable == (char *)-1)
		return (126);
	if (!*executable)
	{
		if (ft_strchr(cmd->args[0], '/'))
			perror(cmd->args[0]);
		else
		{
			ft_putstr_fd(cmd->args[0], STDERR_FILENO);
			ft_putstr_fd(": command not found\n", STDERR_FILENO);
		}
		return (127);
	}
	return (1);
}

int	wait_child(pid_t pid)
{
	int	status;

	waitpid(pid, &status, 0);
	setup_signals_interactive();
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	return (1);
}

void	child_execute_command_node(t_ast_node *node, t_env *env)
{
	if (!node || !node->cmd)
		child_cleanup_and_exit(env, 1);
	if (is_pure_redirect_command(node->cmd))
	{
		ensure_redirect_files_created(node->cmd);
		child_cleanup_and_exit(env, 0);
	}
	if (node->cmd->args && node->cmd->args[0] && node->cmd->args[0][0] != '\0')
	{
		if (is_builtin(node->cmd->args[0]))
			child_execute_builtin(node->cmd, env);
		else
			child_execute_command(node->cmd, env);
	}
	else
		child_cleanup_and_exit(env, 0);
}
