/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_handles.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsarraci <lsarraci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/10 15:57:47 by lsarraci          #+#    #+#             */
/*   Updated: 2026/01/10 16:21:47 by lsarraci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/shell.h"

static int	handle_null_command(void)
{
	return (0);
}

static int	handle_redirect_only(t_command *cmd)
{
	t_redirect	*current;
	int			fd;

	current = cmd->redirects;
	while (current)
	{
		if (current->type == TOKEN_REDIR_OUT)
		{
			fd = open(current->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if (fd == -1)
				return (1);
			close(fd);
		}
		else if (current->type == TOKEN_APPEND)
		{
			fd = open(current->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
			if (fd == -1)
				return (1);
			close(fd);
		}
		current = current->next;
	}
	return (0);
}

static int	handle_builtin_commands(t_command *cmd, t_env *env)
{
	if (cmd->redirects)
		return (execute_redirects(cmd, -1, env));
	return (execute_builtin(cmd->args, env));
}

int	empty_handle_manager(t_command *cmd, t_env *env)
{
	if (!cmd)
		return (handle_null_command());
	if (!cmd->args || !cmd->args[0])
	{
		if (cmd->redirects)
			return (handle_redirect_only(cmd));
		return (handle_null_command());
	}
	if (is_builtin(cmd->args[0]))
		return (handle_builtin_commands(cmd, env));
	return (-1);
}
