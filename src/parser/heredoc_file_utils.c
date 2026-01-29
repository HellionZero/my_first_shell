/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_file_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsarraci <lsarraci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/05 15:52:35 by lsarraci          #+#    #+#             */
/*   Updated: 2026/01/05 16:27:55 by lsarraci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/shell.h"

void	init_heredoc_fd(t_redirect *redir)
{
	if (!redir)
		return ;
	redir->heredoc_fd = -1;
}

int	setup_heredoc(t_redirect *redir)
{
	int	fd;

	if (redir->type != TOKEN_HEREDOC)
		return (0);
	if (redir->heredoc_fd != -1)
		return (1);
	fd = process_heredoc(redir->delimiter);
	if (fd < 0)
	{
		set_exit_status(130);
		return (0);
	}
	redir->heredoc_fd = fd;
	return (1);
}

void	close_heredoc_fd(t_redirect *redir)
{
	if (redir->type == TOKEN_HEREDOC && redir->heredoc_fd >= 0)
	{
		close(redir->heredoc_fd);
		redir->heredoc_fd = -1;
	}
}

void	cleanup_heredoc_fds(t_redirect *redir)
{
	t_redirect	*current;

	current = redir;
	while (current)
	{
		close_heredoc_fd(current);
		current = current->next;
	}
}

int	process_all_heredocs(t_command *cmd)
{
	t_redirect	*current;

	if (!cmd || !cmd->redirects)
		return (1);
	current = cmd->redirects;
	while (current)
	{
		if (current->type == TOKEN_HEREDOC)
		{
			if (!setup_heredoc(current))
			{
				cleanup_heredoc_fds(current);
				return (0);
			}
		}
		current = current->next;
	}
	return (1);
}
