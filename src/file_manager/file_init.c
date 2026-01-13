/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_init.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsarraci <lsarraci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/06 16:15:39 by lsarraci          #+#    #+#             */
/*   Updated: 2026/01/08 15:44:48 by lsarraci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/shell.h"

static char	*build_full_path(const char *dir, const char *cmd)
{
	char	*temp;
	char	*path;

	temp = ft_strjoin(dir, "/");
	if (!temp)
		return (NULL);
	path = ft_strjoin(temp, cmd);
	free(temp);
	return (path);
}

static void	free_path_array(char **paths)
{
	size_t	i;

	if (!paths)
		return ;
	i = 0;
	while (paths[i])
	{
		free(paths[i]);
		i++;
	}
	free(paths);
}

static char	*check_direct_path(char *cmd)
{
	struct stat	path_stat;

	if (ft_strchr(cmd, '/'))
	{
		if (is_directory(cmd))
			return (NULL);
		if (stat(cmd, &path_stat) == 0)
		{
			if (validate_executable(cmd))
				return (ft_strdup(cmd));
			return ((char *)-1);
		}
		return (NULL);
	}
	return (NULL);
}

static char	*search_in_paths(char **paths, char *cmd)
{
	char	*full_path;
	size_t	i;

	i = 0;
	while (paths[i])
	{
		full_path = build_full_path(paths[i], cmd);
		if (full_path && validate_executable(full_path))
		{
			free_path_array(paths);
			return (full_path);
		}
		free(full_path);
		i++;
	}
	free_path_array(paths);
	return (NULL);
}

char	*find_executable(char *cmd, t_env *env)
{
	char	**paths;
	char	*result;

	if (!cmd || !cmd[0])
		return (NULL);
	result = check_direct_path(cmd);
	if (result)
		return (result);
	paths = get_default_path(env);
	if (!paths)
		return (NULL);
	return (search_in_paths(paths, cmd));
}
