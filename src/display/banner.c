/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   banner.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsarraci <lsarraci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 18:56:05 by lsarraci          #+#    #+#             */
/*   Updated: 2025/12/16 18:56:10 by lsarraci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/shell.h"

/*
** Exibe o banner de inicialização do shell
*/
void	display_banner(void)
{
	ft_printf("\n");
	ft_printf("%s%s╔═══════════════════════════════════════╗%s\n",
		COLOR_BOLD, COLOR_CYAN, COLOR_RESET);
	ft_printf("%s%s║                                       ║%s\n",
		COLOR_BOLD, COLOR_CYAN, COLOR_RESET);
	ft_printf("%s%s║        %s🐚  MY SHELL v1.0  🐚%s          ║%s\n",
		COLOR_BOLD, COLOR_CYAN, COLOR_WHITE, COLOR_CYAN, COLOR_RESET);
	ft_printf("%s%s║                                       ║%s\n",
		COLOR_BOLD, COLOR_CYAN, COLOR_RESET);
	ft_printf("%s%s║   %sWelcome to your custom shell!%s       ║%s\n",
		COLOR_BOLD, COLOR_CYAN, COLOR_GREEN, COLOR_CYAN, COLOR_RESET);
	ft_printf("%s%s║                                       ║%s\n",
		COLOR_BOLD, COLOR_CYAN, COLOR_RESET);
	ft_printf("%s%s╚═══════════════════════════════════════╝%s\n",
		COLOR_BOLD, COLOR_CYAN, COLOR_RESET);
	ft_printf("\n%sType 'exit' to quit%s\n\n",
		COLOR_DIM, COLOR_RESET);
}
