/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsarraci <lsarraci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 18:56:21 by lsarraci          #+#    #+#             */
/*   Updated: 2026/01/19 17:53:59 by lsarraci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/shell.h"

static int	should_use_colors(void)
{
	char	*no_color;

	if (!isatty(STDOUT_FILENO))
		return (0);
	no_color = getenv("NO_COLOR");
	if (no_color && no_color[0])
		return (0);
	return (1);
}

static int	supports_utf8(void)
{
	char	*locale;

	locale = getenv("LANG");
	if (!locale)
		locale = getenv("LC_ALL");
	if (!locale)
		locale = getenv("LC_CTYPE");
	if (locale && (ft_strstr(locale, "UTF-8") || ft_strstr(locale, "utf8")))
		return (1);
	return (0);
}

static char	*get_prompt_style(int use_colors, int use_utf8)
{
	char	*style;

	style = getenv("MYSH_PROMPT_STYLE");
	if (style)
	{
		if (ft_strcmp(style, "minimal") == 0)
		{
			if (use_colors)
				return (RL_COLOR_GREEN "minishell" RL_COLOR_RESET " $ ");
			return ("$ ");
		}
		if (ft_strcmp(style, "fancy") == 0 && use_utf8)
		{
			if (use_colors)
				return (RL_COLOR_BOLD_GREEN "minishell" RL_COLOR_RESET ":" \
					RL_COLOR_BOLD_BLUE "~" RL_COLOR_RESET " ➜ ");
			return ("minishell:~ ➜ ");
		}
	}
	if (use_colors)
		return (RL_COLOR_BLUE "minishell" RL_COLOR_RESET ":" \
			RL_COLOR_CYAN "~" RL_COLOR_RESET "> ");
	return ("[minishell]> ");
}

void	ensure_newline_for_prompt(void)
{
	if (!isatty(STDOUT_FILENO))
		return ;
	write(STDOUT_FILENO, CLEAN_LINE, 5);
}

char	*build_prompt(void)
{
	int		use_colors;
	int		use_utf8;

	use_colors = should_use_colors();
	use_utf8 = supports_utf8();
	return (get_prompt_style(use_colors, use_utf8));
}
