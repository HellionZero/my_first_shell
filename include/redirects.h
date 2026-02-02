/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirects.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsarraci <lsarraci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/29 17:25:20 by lsarraci          #+#    #+#             */
/*   Updated: 2026/02/02 15:14:05 by lsarraci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REDIRECTS_H
# define REDIRECTS_H

# include "structs.h"

/* ---------------- Redirection basic operations -----------------*/
int		apply_redir_in(t_redirect *redir);
int		apply_redir_out(t_redirect *redir);
int		apply_append(t_redirect *redir);
int		apply_heredoc(t_redirect *redir);
int		apply_redirects(t_command *cmd);

/* ----------------- Internal redirect helpers ----------------- */
void	cleanup_unused_heredocs(t_command *cmd);
int		apply_single_redirect(t_redirect *redir);
int		validate_out_file(t_redirect *current, t_command *cmd);
int		validate_in_file(t_redirect *current, t_command *cmd);
int		validate_heredoc(t_redirect *current, t_command *cmd);
int		try_apply_redirects(t_command *cmd);
int		save_stdio(int *saved_stdin, int *saved_stdout);
int		process_redirect_item(t_redirect *current, t_command *cmd);
int		empty_handle_manager(t_command *cmd, t_env *env);

#endif
