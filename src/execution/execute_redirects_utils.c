/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_redirects_utils.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsarraci <lsarraci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/29 15:34:19 by lsarraci          #+#    #+#             */
/*   Updated: 2026/01/29 15:48:57 by lsarraci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/shell.h"

void	cleanup_unused_heredocs(t_command *cmd)
{
	t_redirect	*current;

	if (!cmd)
		return ;
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

int	validate_out_file(t_redirect *current, t_command *cmd)
{
	int	flags;
	int	fd;

	flags = O_WRONLY | O_CREAT;
	if (current->type == TOKEN_APPEND)
		flags |= O_APPEND;
	else
		flags |= O_TRUNC;
	fd = open(current->file, flags, 0644);
	if (fd < 0)
	{
		perror(current->file);
		cleanup_unused_heredocs(cmd);
		return (0);
	}
	close(fd);
	return (1);
}

int	validate_in_file(t_redirect *current, t_command *cmd)
{
	int	fd;

	fd = open(current->file, O_RDONLY);
	if (fd < 0)
	{
		perror(current->file);
		cleanup_unused_heredocs(cmd);
		return (0);
	}
	close(fd);
	return (1);
}

int	validate_heredoc(t_redirect *current, t_command *cmd)
{
	if (current->heredoc_fd < 0)
	{
		perror(current->file);
		cleanup_unused_heredocs(cmd);
		return (0);
	}
	return (1);
}

int	process_redirect_item(t_redirect *current, t_command *cmd)
{
	if (current->type == TOKEN_REDIR_OUT || current->type == TOKEN_APPEND)
	{
		if (!validate_out_file(current, cmd))
			return (0);
	}
	else if (current->type == TOKEN_REDIR_IN)
	{
		if (!validate_in_file(current, cmd))
			return (0);
	}
	else if (current->type == TOKEN_HEREDOC)
	{
		if (!validate_heredoc(current, cmd))
			return (0);
	}
	else
	{
		if (!apply_single_redirect(current))
		{
			cleanup_unused_heredocs(cmd);
			return (0);
		}
	}
	return (1);
}
