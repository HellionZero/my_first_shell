/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsarraci <lsarraci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/09 16:23:21 by lsarraci          #+#    #+#             */
/*   Updated: 2026/01/23 14:41:22 by lsarraci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SHELL_H
# define SHELL_H

# include <unistd.h>
# include <stdlib.h>
# include <signal.h>
# include <sys/ioctl.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <fcntl.h>
# include <errno.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "../lib/libft/libft.h"
# include "../lib/ft_printf/ft_printf.h"
# include "builtins.h"
# include "display.h"
# include "exec.h"
# include "lexer.h"
# include "parser.h"
# include "signals.h"
# include "structs.h"
# include "types.h"
# include "lexer.h"
# include "debug.h"
# include "env.h"
# include "file.h"

# define PATH_MAX	4096
# define MAX_COMMAND_LENGTH 131072

/* Exit status management */
void	set_exit_status(int status);
int		get_exit_status(void);

/* Shell initialization and cleanup */
t_env	*init_shell(char **envp);
void	cleanup_shell(t_env *env);

/* Input utilities */
char	*read_line_with_prompt(char *prompt);

/* Shell signal utilities */
int		check_signal_received(void);
int		was_interrupted(void);
void	handle_signal_after_readline(char **input);
void	handle_signal_after_execution(void);
int		should_exit_shell(char *input);
void	shell_cleanup_and_exit(t_env *env, int exit_code);

#endif
