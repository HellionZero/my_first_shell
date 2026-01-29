/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_heredoc.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: loda-sil <loda-sil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/23 17:23:50 by lsarraci          #+#    #+#             */
/*   Updated: 2026/01/13 14:56:18 by loda-sil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/shell.h"

int	is_delimiter_reached(char *line, char *clean_delim)
{
	if (!line)
		return (1);
	if (ft_strcmp(line, clean_delim) == 0)
		return (1);
	return (0);
}

int	process_heredoc(char *delimiter)
{
	int				pipe_fds[2];
	char			*clean_delim;
	t_signal_state	*state;

	pipe(pipe_fds);
	clean_delim = clear_heredoc_delimiter(delimiter);
	signal_enter_heredoc();
	read_heredoc_content(pipe_fds[1], delimiter, clean_delim);
	state = get_signal_state();
	signal_exit_heredoc();
	free(clean_delim);
	close(pipe_fds[1]);
	if (state->received == SIGINT)
	{
		close(pipe_fds[0]);
		return (-1);
	}
	return (pipe_fds[0]);
}
