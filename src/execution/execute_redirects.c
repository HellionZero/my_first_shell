/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_redirects.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsarraci <lsarraci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/09 15:37:29 by lsarraci          #+#    #+#             */
/*   Updated: 2026/01/29 15:46:11 by lsarraci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/shell.h"

static int	apply_redirects_files_only(t_command *cmd)
{
	t_redirect	*current;

	if (!cmd || !cmd->redirects)
		return (1);
	current = cmd->redirects;
	while (current)
	{
		if (!process_redirect_item(current, cmd))
			return (0);
		current = current->next;
	}
	return (1);
}

static int	apply_redirects_with_command(t_command *cmd)
{
	t_redirect	*current;

	if (!cmd || !cmd->redirects)
		return (1);
	current = cmd->redirects;
	while (current)
	{
		if (!apply_single_redirect(current))
		{
			cleanup_unused_heredocs(cmd);
			perror("redirect");
			return (0);
		}
		current = current->next;
	}
	cleanup_unused_heredocs(cmd);
	return (1);
}

int	apply_redirects(t_command *cmd)
{
	if (!cmd || !cmd->redirects)
		return (1);
	if (!cmd->args || !cmd->args[0] || cmd->args[0][0] == '\0')
		return (apply_redirects_files_only(cmd));
	else
		return (apply_redirects_with_command(cmd));
}

int	execute_redirects(t_command *command, int fd, t_env *env)
{
	int	saved_stdin;
	int	saved_stdout;
	int	exit_status;

	if (!command->args || !command->args[0] || command->args[0][0] == '\0')
	{
		if (!apply_redirects_files_only(command))
			return (1);
		ensure_redirect_files_created(command);
		return (0);
	}
	if (!save_stdio(&saved_stdin, &saved_stdout))
		return (1);
	resolve_fd(&fd);
	if (!apply_redirects_with_command(command))
		return (close_and_exit(saved_stdin, saved_stdout, 1));
	exit_status = execute_builtin(command->args, env);
	return (close_and_exit(saved_stdin, saved_stdout, exit_status));
}
