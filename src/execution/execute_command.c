/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_command.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsarraci <lsarraci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/02 14:54:59 by lsarraci          #+#    #+#             */
/*   Updated: 2026/01/26 16:26:11 by lsarraci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/shell.h"

static int	wait_child(pid_t pid)
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

static int	fork_and_execute(t_command *cmd, char *executable, t_env *env)
{
	pid_t	pid;

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
		child_execute_command(cmd, env);
	free(executable);
	return (wait_child(pid));
}

static int	set_command_code(t_command *cmd, char **executable)
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

static char	*get_command_executable(t_command *cmd, t_env *env)
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

int	execute_command(t_command *cmd, t_env *env)
{
	char	*executable;
	int		handled;
	int		dir_code;
	int		command_code;

	if (!cmd->args[0] || cmd->args[0][0] == '\0')
		return (0);
	handled = empty_handle_manager(cmd, env);
	if (handled != -1)
		return (handled);
	if (is_builtin(cmd->args[0]) && cmd->redirects)
		return (execute_redirects(cmd, -1, env));
	if (is_builtin(cmd->args[0]) && !env->in_pipeline)
		return (execute_builtin(cmd->args, env));
	executable = get_command_executable(cmd, env);
	command_code = set_command_code(cmd, &executable);
	if (command_code != 1)
		return (command_code);
	dir_code = is_directory_error(executable);
	if (command_code != 1 || dir_code != -1)
	{
		if (dir_code != -1)
			command_code = dir_code;
		free(executable);
		return (command_code);
	}
	return (fork_and_execute(cmd, executable, env));
}
