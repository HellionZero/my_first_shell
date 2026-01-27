/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_command.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsarraci <lsarraci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/02 14:54:59 by lsarraci          #+#    #+#             */
/*   Updated: 2026/01/27 19:24:21 by lsarraci         ###   ########.fr       */
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

void	ensure_redirect_files_created(t_command *cmd)
{
	t_redirect	*r;
	int			flags;
	int			fd;
	int			no_cmd;

	fd = -1;
	if (cmd->redirects)
		r = cmd->redirects;
	else
		r = NULL;
	no_cmd = (!cmd || !cmd->args || !cmd->args[0] || cmd->args[0][0] == '\0');
	if (!no_cmd)
		return ;
	while (r)
	{
		if (r->type == TOKEN_REDIR_OUT || r->type == TOKEN_APPEND)
		{
			if (r->type == TOKEN_APPEND)
				flags = O_WRONLY | O_CREAT | O_APPEND;
			else
				flags = O_WRONLY | O_CREAT | O_TRUNC;
			fd = open(r->file, flags, 0644);
			if (fd >= 0)
				close(fd);
		}
		else if (r->type == TOKEN_REDIR_IN)
		{
			fd = open(r->file, O_RDONLY);
			if (fd >= 0)
				close(fd);
		}
		else if (r->type == TOKEN_HEREDOC)
		{
			if (r->heredoc_fd >= 0)
				close(r->heredoc_fd);
			r->heredoc_fd = -1;
		}
		r = r->next;
	}
}

int	execute_command(t_command *cmd, t_env *env)
{
	char	*executable;
	int		handled;
	int		command_code;

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
		executable = get_command_executable(cmd, env);
		command_code = set_command_code(cmd, &executable);
		if (command_code != 1)
			return (command_code);
		if (is_directory_error(executable) == 126)
		{
			free(executable);
			return (126);
		}
		return (fork_and_execute(cmd, executable, env));
	}
	return (0);
}
