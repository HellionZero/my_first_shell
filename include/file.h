/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsarraci <lsarraci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/06 16:16:30 by lsarraci          #+#    #+#             */
/*   Updated: 2026/01/26 15:47:02 by lsarraci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FILE_H
# define FILE_H

# include "structs.h"

/* ---------------   Path Management  ----------------------*/
char	*find_executable(char *cmd, t_env *env);
char	**get_default_path(t_env *env);

/* ---------------   File Utilities  -----------------------*/
int		is_directory(const char *path);
int		is_directory_error(char *path);
int		validate_executable(char *path);

#endif
