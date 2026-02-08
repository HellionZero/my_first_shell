/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirects.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsarraci <lsarraci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/29 17:25:20 by lsarraci          #+#    #+#             */
/*   Updated: 2026/02/05 17:00:43 by lsarraci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REDIRECTS_H
# define REDIRECTS_H

# include "structs.h"

/* ---------------- Redirection basic operations -----------------*/

/* function to apply input redirection.
checks if there's a redirect, and applies it. 
if the file descriptor is different from in, 
it duplicates the file descriptor. this 
behavior is set to prevent unwanted side effects.*/
int		apply_redir_in(t_redirect *redir);

/* function to apply output redirection.
checks if there's a redirect, and applies it.
if the file descriptor is different from out, 
it duplicates the file descriptor. */
int		apply_redir_out(t_redirect *redir);

/* function to apply append redirection.
checks if there's a redirect, and applies it. 
if the file descriptor is different from out, 
it duplicates the file descriptor of the out */
int		apply_append(t_redirect *redir);

/* function to apply heredoc redirection.
checks if there's a heredoc redirect, and applies it. 
if the fd is different from in,
it duplicates the file descriptor. */
int		apply_heredoc(t_redirect *redir);

/* function to apply all redirects for a command.
checks if there're are valid arguments and
applies the redirects accordingly. */
int		apply_redirects(t_command *cmd);

/* ----------------- Redirection validation ----------------- */

/* function to validate the out file. */
int		validate_out_file(t_redirect *current, t_command *cmd);

/* function to validate the append file. */
int		validate_in_file(t_redirect *current, t_command *cmd);

/* function to validate the heredoc file. */
int		validate_heredoc(t_redirect *current, t_command *cmd);

/* ----------------- Internal redirect helpers ----------------- */

/* special function to check if there're unused heredocs 
and clean them up. */
void	cleanup_unused_heredocs(t_command *cmd);

/* function to apply a single redirect based on its type. */
int		apply_single_redirect(t_redirect *redir);

/* function to process a redirect item for file creation. 
this function needs to store data from the 
standard input and output file descriptors. */
int		save_stdio(int *saved_stdin, int *saved_stdout);

/* function to process a redirect item for file creation. */
int		process_redirect_item(t_redirect *current, t_command *cmd);

/* function to manage empty handles.
redirects cannot have empty handles. */
int		empty_handle_manager(t_command *cmd, t_env *env);

#endif
