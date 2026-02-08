/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsarraci <lsarraci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/06 16:16:30 by lsarraci          #+#    #+#             */
/*   Updated: 2026/02/05 15:34:26 by lsarraci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FILE_H
# define FILE_H

# include "structs.h"

/* ---------------   Path Management  ----------------------*/

/* function to find an executable in the system path */
char	*find_executable(char *cmd, t_env *env);

/* function to get the default system path */
char	**get_default_path(t_env *env);

/* ---------------   File Utilities  -----------------------*/

/* verify if the path is a directory */
int		is_directory(const char *path);

/* verify if accessing the path causes a directory error */
int		is_directory_error(char *path);

/* validate if the executable at the given path can be run */
int		validate_executable(char *path);

#endif
