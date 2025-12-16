/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsarraci <lsarraci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 18:57:17 by lsarraci          #+#    #+#             */
/*   Updated: 2025/12/16 18:57:22 by lsarraci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIGNALS_H
# define SIGNALS_H

# include <signal.h>

/* ---------------   signals setup -------------------------*/

void	setup_signals_interactive(void);
void	setup_signals_executing(void);
void	setup_signals_heredoc(void);

/* ---------------signal utilities -------------------------*/

void	handle_sigint(int sig);
void	handle_sigquit(int sig);
void	restore_signals_default(void);

#endif
