/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsarraci <lsarraci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 17:09:23 by lsarraci          #+#    #+#             */
/*   Updated: 2025/11/03 15:45:37 by lsarraci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*join_and_free(char	*store, char *buffer)
{
	char	*tmp;

	tmp = ft_strjoin(store, buffer);
	if (!tmp)
	{
		free(store);
		return (NULL);
	}
	free(store);
	return (tmp);
}

static char	*read_file(int fd, char *remain)
{
	char	*buffer;
	int		ch_read;

	if (!remain)
		remain = ft_calloc(1, 1);
	if (remain && ft_strchr(remain, '\n'))
		return (remain);
	buffer = ft_calloc(BUFFER_SIZE + 1, sizeof(char));
	ch_read = 1;
	while (ch_read > 0 && !ft_strchr(remain, '\n'))
	{
		ch_read = read(fd, buffer, BUFFER_SIZE);
		if (ch_read == -1)
			return (free(remain), free(buffer), NULL);
		buffer[ch_read] = '\0';
		remain = join_and_free(remain, buffer);
		if (!remain)
			return (free(buffer), NULL);
	}
	free(buffer);
	return (remain);
}

static char	*return_line(char *line)
{
	char	*tmp;
	int		i;

	i = 0;
	if (!line[i])
		return (NULL);
	while (line[i] && line[i] != '\n')
		i++;
	tmp = ft_calloc(i + 2, sizeof(char));
	i = 0;
	while (line[i] && line[i] != '\n')
	{
		tmp[i] = line[i];
		i++;
	}
	if (line[i] && line[i] == '\n')
		tmp[i++] = '\n';
	return (tmp);
}

static char	*get_remain(char *line)
{
	char	*remain;
	int		i;

	i = 0;
	while (line[i] && line[i] != '\n')
		i++;
	if (!line[i])
	{
		free(line);
		return (NULL);
	}
	remain = ft_substr(line, i + 1, (ft_strlen(line) - i));
	if (!remain)
	{
		free(line);
		return (NULL);
	}
	free(line);
	return (remain);
}

char	*get_next_line(int fd)
{
	static char	*buffer;
	char		*line;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	buffer = read_file(fd, buffer);
	if (!buffer)
		return (NULL);
	line = return_line(buffer);
	if (!line)
	{
		free(buffer);
		buffer = NULL;
		return (NULL);
	}
	buffer = get_remain(buffer);
	return (line);
}
