/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsarraci <lsarraci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/09 16:23:21 by lsarraci          #+#    #+#             */
/*   Updated: 2026/02/09 15:32:05 by lsarraci         ###   ########.fr       */
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

/* ----------------- Exit status management ------------------*/

/* Sets the exit status of the last executed command. 
it initializes the exit status to 0 and updates it 
accordingly to the provided status of the last executed command */
void	set_exit_status(int status);

/* Retrieves the exit status of the last executed command */
int		get_exit_status(void);

/* ----------------- Shell initialization and cleanup ------------------*/

/* Prepares the shell environment and return the 
list of environment variables. it opens the fd located
 at /dev/tty and reads from it.
It initializes the banner and verifies if the 
input is from an interactive terminal. */
t_env	*init_shell(char **envp);

/* Cleans up the environment and shell resources */
void	cleanup_shell(t_env *env);

/* ----------------Input utilities ------------------*/

/* function to read a line of input from the user 
with a given prompt from an interactive terminal. */
char	*read_line_with_prompt(const char *prompt);

/* ----------------- Shell signal utilities ------------------*/

/* function to verify if a signal was received */
int		check_signal_received(void);

/* function to check if the shell was interrupted by a signal */
int		was_interrupted(void);

/* function to handle signals after readline */
void	handle_signal_after_readline(char **input);

/* function to handle signals after command execution */
void	handle_signal_after_execution(void);

/* function to determine if the shell should exit, checking the input. */
int		should_exit_shell(char *input);

/* Shell cleanup and exit. it performs the cleanup 
of resources of the shell and clear the history 
of commands of interactive mode */
void	shell_cleanup_and_exit(t_env *env, int exit_code);

#endif
