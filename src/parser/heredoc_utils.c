/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsarraci <lsarraci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/23 16:44:11 by lsarraci          #+#    #+#             */
/*   Updated: 2026/01/19 16:02:39 by lsarraci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/shell.h"

char	*clear_heredoc_delimiter(char *delimiter)
{
	char	*cleaned;

	if (!delimiter)
		return (NULL);
	if (has_quotes(delimiter[0]))
		cleaned = ft_strtrim(delimiter, "\'\"");
	else
		cleaned = ft_strdup(delimiter);
	return (cleaned);
}

char	*extract_var_name_heredoc(char *start, int *len)
{
	int		i;
	char	*var_name;

	i = 0;
	if (!ft_isalpha(start[i]) && start[i] != '_')
	{
		*len = 0;
		return (ft_strdup(""));
	}
	while (start[i] && is_var_char(start[i]))
		i++;
	*len = i;
	var_name = ft_substr(start, 0, i);
	return (var_name);
}

static int	check_delimeter(char *line, char *clean_delim)
{
	if (is_delimiter_reached(line, clean_delim))
	{
		if (!line || line[0] == '\0')
			return (0);
		free(line);
		return (1);
	}
	return (0);
}

static int	check_line_with_prompt(char **line_ptr, char *prompt)
{
	char	*line;

	line = read_line_with_prompt(prompt);
	if (!line)
		return (0);
	*line_ptr = line;
	return (1);
}

void	read_heredoc_content(int pipe_fd, char *delimiter,
	char *clean_delim)
{
	char			*line;
	char			*expanded;
	t_signal_state	*state;

	state = get_signal_state();
	while (1)
	{
		if (state->received == SIGINT)
			break ;
		if (check_line_with_prompt(&line, "> ") == 0)
			break ;
		if (check_delimeter(line, clean_delim))
			break ;
		expanded = get_expanded_line(line, delimiter);
		write_line_to_pipe(pipe_fd, expanded);
		if (expanded != line)
			free(expanded);
		free(line);
	}
}
