/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_expansion_utils.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: loda-sil <loda-sil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/11 15:51:01 by lsarraci          #+#    #+#             */
/*   Updated: 2026/01/13 14:57:04 by loda-sil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/shell.h"

static char	*process_variable(char *line, int *i)
{
	char	*var_name;
	char	*var_value;
	int		var_len;

	var_name = extract_var_name_heredoc(&line[*i + 1], &var_len);
	var_value = expand_variable(var_name);
	free(var_name);
	*i += var_len + 1;
	return (var_value);
}

int	should_expand_heredoc(char *delimiter)
{
	if (!delimiter)
		return (0);
	return (!has_quotes(delimiter[0]));
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
		line = read_line_with_prompt("> ");
		if (state->received == SIGINT)
		{
			if (line)
				free(line);
			break ;
		}
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

char	*get_expanded_line(char *line, char *delimiter)
{
	if (should_expand_heredoc(delimiter))
		return (expand_heredoc_line(line));
	else
		return (line);
}

char	*expand_heredoc_line(char *line)
{
	char	*result;
	char	*var_value;
	int		i;

	if (!line || !needs_expansion(line))
		return (ft_strdup(line));
	result = ft_strdup("");
	i = 0;
	while (line[i])
	{
		if (line[i] == '$' && line[i + 1] && is_var_char(line[i + 1]))
		{
			var_value = process_variable(line, &i);
			result = join_and_free(result, var_value);
			free(var_value);
		}
		else
			result = join_char_and_free(result, line[i++]);
	}
	return (result);
}
