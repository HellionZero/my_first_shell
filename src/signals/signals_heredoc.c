/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals_heredoc.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsarraci <lsarraci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/08 14:21:19 by lsarraci          #+#    #+#             */
/*   Updated: 2026/01/08 14:25:29 by lsarraci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/shell.h"

void	signal_enter_heredoc(void)
{
	t_signal_state	*state;

	state = get_signal_state();
	state->in_heredoc = 1;
	state->received = 0;
	setup_signals_heredoc();
}

void	signal_exit_heredoc(void)
{
	t_signal_state	*state;

	state = get_signal_state();
	state->in_heredoc = 0;
	setup_signals_interactive();
}
