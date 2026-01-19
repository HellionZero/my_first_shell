/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_redirects.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsarraci <lsarraci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/09 15:37:29 by lsarraci          #+#    #+#             */
/*   Updated: 2026/01/19 15:59:22 by lsarraci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/shell.h"

static int	apply_single_redirect(t_redirect *redir)
{
	if (redir->type == TOKEN_REDIR_IN)
		return (apply_redir_in(redir));
	else if (redir->type == TOKEN_REDIR_OUT)
		return (apply_redir_out(redir));
	else if (redir->type == TOKEN_APPEND)
		return (apply_append(redir));
	else if (redir->type == TOKEN_HEREDOC)
		return (apply_heredoc(redir));
	return (1);
}

static int	apply_child_redirects(t_redirect *redir, t_command *cmd, t_env *env)
{
	pid_t	pid;

	if (!cmd || !cmd->redirects)
		return (1);
	pid = fork();
	if (pid == -1)
		return (perror("fork"), 1);
	if (pid == 0)
	{
		if (!apply_single_redirect(redir))
			child_cleanup_and_exit(env, 1);
		child_cleanup_and_exit(env, 0);
		free_env(env);
		exit(0);
	}
	waitpid(pid, NULL, 0);
	return (1);
}

static void	cleanup_unused_heredocs(t_command *cmd)
{
	t_redirect	*current;

	current = cmd->redirects;
	while (current)
	{
		if (current->type == TOKEN_HEREDOC && current->heredoc_fd >= 0)
		{
			close(current->heredoc_fd);
			current->heredoc_fd = -1;
		}
		current = current->next;
	}
}

int	apply_redirects(t_command *cmd)
{
	t_redirect	*current;

	if (!cmd)
		return (1);
	current = cmd->redirects;
	while (current)
	{
		if (!apply_single_redirect(current))
		{
			cleanup_unused_heredocs(cmd);
			return (0);
		}
		else if (!apply_child_redirects(current, cmd, NULL))
		{
			cleanup_unused_heredocs(cmd);
			return (0);
		}
		current = current->next;
	}
	cleanup_unused_heredocs(cmd);
	return (1);
}

int	execute_redirects(t_command *command, int fd, t_env *env)
{
	int	saved_stdin;
	int	saved_stdout;
	int	exit_status;

	saved_stdin = dup(0);
	if (saved_stdin < 0)
		return (1);
	saved_stdout = dup(1);
	if (saved_stdout < 0)
	{
		close(saved_stdin);
		return (1);
	}
	resolve_fd(&fd);
	if (!apply_redirects(command))
		return (close_and_exit(saved_stdin, saved_stdout, 1));
	exit_status = execute_builtin(command->args, env);
	return (close_and_exit(saved_stdin, saved_stdout, exit_status));
}
