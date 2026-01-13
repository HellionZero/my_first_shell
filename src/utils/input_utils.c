/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsarraci <lsarraci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/11 16:11:03 by lsarraci          #+#    #+#             */
/*   Updated: 2026/01/11 16:11:08 by lsarraci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/shell.h"

char	*read_line_with_prompt(char *prompt)
{
	char			*line;
	size_t			len;
	t_signal_state	*state;

	state = get_signal_state();
	if (state->in_heredoc || !isatty(STDIN_FILENO))
	{
		if (prompt && isatty(STDIN_FILENO))
			ft_printf("%s", prompt);
		line = get_next_line(STDIN_FILENO);
		if (!line)
			return (NULL);
		len = ft_strlen(line);
		if (len > 0 && line[len - 1] == '\n')
			line[len - 1] = '\0';
		return (line);
	}
	return (readline(prompt));
}
