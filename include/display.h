/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsarraci <lsarraci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 18:57:36 by lsarraci          #+#    #+#             */
/*   Updated: 2025/12/16 18:57:40 by lsarraci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DISPLAY_H
# define DISPLAY_H

/*
** Códigos de cores ANSI
*/
# define COLOR_RESET	"\033[0m"
# define COLOR_BOLD		"\033[1m"
# define COLOR_DIM		"\033[2m"

# define COLOR_BLACK	"\033[30m"
# define COLOR_RED		"\033[31m"
# define COLOR_GREEN	"\033[32m"
# define COLOR_YELLOW	"\033[33m"
# define COLOR_BLUE		"\033[34m"
# define COLOR_MAGENTA	"\033[35m"
# define COLOR_CYAN		"\033[36m"
# define COLOR_WHITE	"\033[37m"

/*
** Códigos de cores para prompts readline
** \001 (RL_PROMPT_START_IGNORE) e \002 (RL_PROMPT_END_IGNORE)
** CADA sequência de escape deve estar completamente envolvida
*/
# define RL_COLOR_RESET		"\001\033[0m\002"
# define RL_COLOR_BOLD		"\001\033[1m\002"
# define RL_COLOR_GREEN		"\001\033[32m\002"
# define RL_COLOR_BLUE		"\001\033[34m\002"
# define RL_COLOR_CYAN		"\001\033[36m\002"

/*
** Macros auxiliares para facilitar uso no prompt
*/

# define COLOR_BG_BLACK		"\033[40m"
# define COLOR_BG_RED		"\033[41m"
# define COLOR_BG_GREEN		"\033[42m"
# define COLOR_BG_YELLOW	"\033[43m"
# define COLOR_BG_BLUE		"\033[44m"
# define COLOR_BG_MAGENTA	"\033[45m"
# define COLOR_BG_CYAN		"\033[46m"
# define COLOR_BG_WHITE		"\033[47m"

/*
** Funções de display
*/
void	display_banner(void);
char	*get_colored_prompt(void);

#endif
