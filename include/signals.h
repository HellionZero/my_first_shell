/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsarraci <lsarraci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 18:57:17 by lsarraci          #+#    #+#             */
/*   Updated: 2026/02/02 14:30:04 by lsarraci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIGNALS_H
# define SIGNALS_H

# include "structs.h"

/* ---------------   signals setup -------------------------*/

/* Signal handler for interactive mode.
handles SIGINT and SIGQUIT. 
(ctrl+C) resets the input line and 
(ctrl+\\) doesn't do anything. This setup ensures that
the shell remains responsive to user interrupts without
terminating the session. */
void			setup_signals_interactive(void);

/* Signal handler for executing commands. 
each command runs with these settings, 
isolated from interactive mode signals */
void			setup_signals_executing(void);

/* Special signal handler for heredoc input.
it handles SIGINT and ignores SIGQUIT, and uses (ctrl+C) 
to interrupt heredoc input, while (ctrl+\\) is used here to 
ignore quit signals.
(ctrl+D) is used to signal end-of-file for heredoc input,
allowing users to terminate input without affecting other 
parts of the main shell process. */
void			setup_signals_heredoc(void);

/* ---------------signal utilities -------------------------*/

/* function to handle SIGINT signal (Ctrl+C) */
void			handle_sigint(int sig);

/* function to restore default signal handlers. 
it's used when the shell needs to revert to the original 
signal behavior, typically before executing a new program */
void			restore_signals_default(void);

/* ---------------signal state management ------------------*/

/* singleton to manage signal states. */
t_signal_state	*get_signal_state(void);

/* clears the received signal state */
void			signal_clear(void);

/*signal handler to enable heredoc mode 
mantaining other signal states isolated */
void			signal_enter_heredoc(void);

/* function to exit heredoc mode and restore previous signal states */
void			signal_exit_heredoc(void);

#endif
