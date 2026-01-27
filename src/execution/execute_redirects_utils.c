/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_redirects_utils.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsarraci <lsarraci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/09 15:52:08 by lsarraci          #+#    #+#             */
/*   Updated: 2026/01/27 18:32:48 by lsarraci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/shell.h"

int	apply_redir_in(t_redirect *redir)
{
	int	fd;

	fd = open(redir->file, O_RDONLY);
	if (fd < 0)
	{
		perror(redir->file);
		return (0);
	}
	if (fd != STDIN_FILENO)
		dup2(fd, STDIN_FILENO);
	close(fd);
	return (1);
}

int	apply_redir_out(t_redirect *redir)
{
	int	fd;

	fd = open(redir->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
	{
		perror(redir->file);
		return (0);
	}
	if (fd != STDOUT_FILENO)
		dup2(fd, STDOUT_FILENO);
	close(fd);
	return (1);
}

int	apply_append(t_redirect *redir)
{
	int	fd;

	fd = open(redir->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd < 0)
	{
		perror(redir->file);
		return (0);
	}
	if (fd != STDOUT_FILENO)
		dup2(fd, STDOUT_FILENO);
	close(fd);
	return (1);
}

int	apply_heredoc(t_redirect *redir)
{
	if (redir->heredoc_fd < 0)
	{
		ft_printf("minishell: heredoc error\n");
		return (0);
	}
	if (redir->heredoc_fd != STDIN_FILENO)
		dup2(redir->heredoc_fd, STDIN_FILENO);
	close(redir->heredoc_fd);
	redir->heredoc_fd = -1;
	return (1);
}
