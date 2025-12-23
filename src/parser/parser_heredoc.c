/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_heredoc.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsarraci <lsarraci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/23 17:23:50 by lsarraci          #+#    #+#             */
/*   Updated: 2025/12/23 17:40:38 by lsarraci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/shell.h"

static void	write_line_to_pipe(int fd, char *line)
{
	write(fd, line, ft_strlen(line));
	write(fd, "\n", 1);
}

static char	*get_expanded_line(char *line, char *delimiter)
{
	if (should_expand_heredoc(delimiter))
		return (expand_heredoc_line(line));
	else
		return (line);
}

static int	is_delimiter_reached(char *line, char *clean_delim)
{
	if (!line)
		return (1);
	if (ft_strcmp(line, clean_delim) == 0)
		return (1);
	return (0);
}

static void	read_heredoc_content(int pipe_fd, char *delimiter,
	char *clean_delim)
{
	char	*line;
	char	*expanded;

	while (1)
	{
		line = readline("> ");
		if (is_delimiter_reached(line, clean_delim))
		{
			if (line)
				free(line);
			break ;
		}
		expanded = get_expanded_line(line, delimiter);
		write_line_to_pipe(pipe_fd, expanded);
		if (expanded != line)
			free(expanded);
		free(line);
	}
}

int	process_heredoc(char *delimiter)
{
	int		pipe_fds[2];
	char	*clean_delim;

	pipe(pipe_fds);
	clean_delim = clear_heredoc_delimiter(delimiter);
	read_heredoc_content(pipe_fds[1], delimiter, clean_delim);
	free(clean_delim);
	close(pipe_fds[1]);
	return (pipe_fds[0]);
}
