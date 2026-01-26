/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsarraci <lsarraci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/08 15:26:14 by lsarraci          #+#    #+#             */
/*   Updated: 2026/01/26 16:23:25 by lsarraci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/shell.h"

char	**get_default_path(t_env *env)
{
	char		*path_value;
	char		**paths;
	t_env_var	*path_var;

	path_var = env_get(env, "PATH");
	if (!path_var || !path_var->value)
	{
		paths = ft_split("/usr/local/bin:/usr/bin:/bin:/usr/sbin:/sbin",
				':');
		return (paths);
	}
	path_value = path_var->value;
	paths = ft_split(path_value, ':');
	return (paths);
}

int	is_directory(const char *path)
{
	t_stat	path_stat;

	if (stat(path, &path_stat) != 0)
		return (0);
	return (S_ISDIR(path_stat.st_mode));
}

int	validate_executable(char *path)
{
	if (access(path, X_OK) == 0)
		return (1);
	if (access(path, F_OK) == 0 && access(path, X_OK) != 0)
	{
		set_exit_status(126);
		return (126);
	}
	return (0);
}

int	is_directory_error(char *path)
{
	struct stat	st;

	if (stat(path, &st) == 0 && S_ISDIR(st.st_mode))
	{
		ft_putstr_fd(path, STDERR_FILENO);
		ft_putstr_fd(": Is a directory\n", STDERR_FILENO);
		set_exit_status(126);
		return (126);
	}
	return (-1);
}
