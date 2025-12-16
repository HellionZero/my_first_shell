/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsarraci <lsarraci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 18:56:21 by lsarraci          #+#    #+#             */
/*   Updated: 2025/12/16 18:58:14 by lsarraci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/shell.h"

/*
** Retorna um prompt sem usar defines - construção manual
** Formato: [my_shell] > em verde
** \001 = início de sequência invisível
** \002 = fim de sequência invisível
** \033[32m = verde
** \033[0m = reset
*/
char	*get_colored_prompt(void)
{
	return ("\001\033[32m\002[my_shell]\001\033[0m\002 > ");
}
