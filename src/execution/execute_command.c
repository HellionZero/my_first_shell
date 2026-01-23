/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_command.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsarraci <lsarraci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/02 14:54:59 by lsarraci          #+#    #+#             */
/*   Updated: 2026/01/23 17:33:14 by lsarraci         ###   ########.fr       */
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

static int	is_directory_error(char *path)
{
	struct stat	st;

	if (stat(path, &st) == 0 && S_ISDIR(st.st_mode))
	{
		ft_putstr_fd(path, STDERR_FILENO);
		ft_putstr_fd(": Is a directory\n", STDERR_FILENO);
		return (126);
	}
	return (-1);
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

int	execute_command(t_command *cmd, t_env *env)
{
	char	*executable;
	int		handled;
	int		dir_code;

	if (!cmd->args[0] || cmd->args[0][0] == '\0')
		return (0);
	handled = empty_handle_manager(cmd, env);
	if (handled != -1)
		return (handled);
	if (is_builtin(cmd->args[0]) && cmd->redirects)
		return (execute_redirects(cmd, -1, env));
	if (is_builtin(cmd->args[0]) && !env->in_pipeline)
		return (execute_builtin(cmd->args, env));
	executable = define_executable(cmd, env);
	if (executable == (char *)-1)
		return (126);
	if (!executable)
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
	dir_code = is_directory_error(executable);
	if (dir_code != -1)
	{
		free(executable);
		return (dir_code);
	}
	return (fork_and_execute(cmd, executable, env));
}
