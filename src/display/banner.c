/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   banner.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsarraci <lsarraci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 18:56:05 by lsarraci          #+#    #+#             */
/*   Updated: 2026/01/11 16:24:39 by lsarraci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/shell.h"

void	display_banner(void)
{
	ft_printf("\n");
	ft_printf("%s%s╔═══════════════════════════════════════╗%s\n",
		COLOR_BOLD, COLOR_RED, COLOR_RESET);
	ft_printf("%s%s║                                       ║%s\n",
		COLOR_BOLD, COLOR_RED, COLOR_RESET);
	ft_printf("%s%s║        %s🐚  MINISHELL v1.0  🐚%s         ║%s\n",
		COLOR_BOLD, COLOR_RED, COLOR_WHITE, COLOR_RED, COLOR_RESET);
	ft_printf("%s%s║                                       ║%s\n",
		COLOR_BOLD, COLOR_RED, COLOR_RESET);
	ft_printf("%s%s║     %sWelcome to our custom shell!%s      ║%s\n",
		COLOR_BOLD, COLOR_RED, COLOR_GREEN, COLOR_RED, COLOR_RESET);
	ft_printf("%s%s║                                       ║%s\n",
		COLOR_BOLD, COLOR_RED, COLOR_RESET);
	ft_printf("%s%s║ %sBrought to you by lsarraci & loda-sil%s ║%s\n",
		COLOR_BOLD, COLOR_RED, COLOR_GREEN, COLOR_RED, COLOR_RESET);
	ft_printf("%s%s╚═══════════════════════════════════════╝%s\n",
		COLOR_BOLD, COLOR_RED, COLOR_RESET);
	ft_printf("\n%sType 'exit' or press Ctrl+D to quit%s\n\n",
		COLOR_DIM, COLOR_RESET);
}
