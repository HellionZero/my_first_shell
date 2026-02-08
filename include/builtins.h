/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsarraci <lsarraci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 15:08:41 by lsarraci          #+#    #+#             */
/*   Updated: 2026/02/05 17:35:43 by lsarraci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H

# include "structs.h"

/*--------- builtin commands -----------------*/

/* cd: change directory. it needs a path argument. 
if none is provided, it changes to the home directory.*/
int	builtin_cd(char **args, t_env *env);

/* echo: print arguments to standard output.
if no arguments are provided, it prints a newline.*/
int	builtin_echo(char **args, t_env *env);

/* env: print the current environment variables. */
int	builtin_env(char **args, t_env *env);

/* exit: exit the shell.
it can take an optional exit status argument. */
int	builtin_exit(char **args, t_env *env);

/* export: set environment variables. */
int	builtin_export(char **args, t_env *env);

/* pwd: print the current working directory. */
int	builtin_pwd(char **args, t_env *env);

/* unset: remove environment variables. */
int	builtin_unset(char **args, t_env *env);

#endif
