/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsarraci <lsarraci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 18:56:21 by lsarraci          #+#    #+#             */
/*   Updated: 2026/02/09 15:35:53 by lsarraci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/shell.h"

const char	*build_prompt(void)
{
	return (COLOR_BLUE"[minishell]> "COLOR_RESET);
}

void	ensure_newline_for_prompt(void)
{
	if (!isatty(STDIN_FILENO))
		return ;
	write(STDOUT_FILENO, CLEAN_LINE, sizeof(CLEAN_LINE) - 1);
}
