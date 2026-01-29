/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsarraci <lsarraci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/23 17:56:03 by lsarraci          #+#    #+#             */
/*   Updated: 2026/01/19 15:47:17 by lsarraci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/shell.h"

int	has_redirect_without_target(t_token *tokens)
{
	t_token	*current;

	current = tokens;
	while (current)
	{
		if (is_redirect(current) && !redirect_has_valid_target(current))
			return (1);
		current = current->next;
	}
	return (0);
}

int	has_redirect_before_operator(t_token *tokens)
{
	t_token	*current;

	current = tokens;
	while (current && current->next)
	{
		if (is_invalid_redirect_sequence(current))
			return (1);
		current = current->next;
	}
	return (0);
}

int	has_heredoc(t_command *cmd)
{
	t_redirect	*current;

	if (!cmd || !cmd->redirects)
		return (0);
	current = cmd->redirects;
	while (current)
	{
		if (current->type == TOKEN_HEREDOC)
			return (1);
		current = current->next;
	}
	return (0);
}

void	write_line_to_pipe(int fd, char *line)
{
	write(fd, line, ft_strlen(line));
	write(fd, "\n", 1);
}
