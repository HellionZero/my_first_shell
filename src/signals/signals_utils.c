/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsarraci <lsarraci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/08 13:56:05 by lsarraci          #+#    #+#             */
/*   Updated: 2026/02/02 14:27:32 by lsarraci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/shell.h"

t_signal_state	*get_signal_state(void)
{
	static t_signal_state	signal_state = {0, 0, 0};

	return (&signal_state);
}

void	signal_clear(void)
{
	t_signal_state	*state;

	state = get_signal_state();
	state->received = 0;
}
