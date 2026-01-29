/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsarraci <lsarraci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 18:57:36 by lsarraci          #+#    #+#             */
/*   Updated: 2026/01/11 19:00:24 by lsarraci         ###   ########.fr       */
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

# define RL_COLOR_RESET			"\001\033[0m\002"
# define RL_COLOR_BOLD			"\001\033[1m\002"
# define RL_COLOR_GREEN			"\001\033[32m\002"
# define RL_COLOR_BLUE			"\001\033[34m\002"
# define RL_COLOR_CYAN			"\001\033[36m\002"
# define RL_COLOR_BOLD_GREEN	"\001\033[1;32m\002"
# define RL_COLOR_BOLD_BLUE		"\001\033[1;34m\002"

# define CURSOR_ESCAPE			"\033[6n"
# define CLEAN_LINE				"\r\033[K"

/* Display functions */
void				display_banner(void);
void				ensure_newline_for_prompt(void);
char				*build_prompt(void);

#endif
