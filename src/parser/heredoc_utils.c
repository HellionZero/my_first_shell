/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsarraci <lsarraci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/23 16:44:11 by lsarraci          #+#    #+#             */
/*   Updated: 2025/12/23 18:09:46 by lsarraci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/shell.h"

int	should_expand_heredoc(char *delimiter)
{
	if (!delimiter)
		return (0);
	return (!has_quotes(delimiter[0]));
}

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
