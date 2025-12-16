/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsarraci <lsarraci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/09 16:23:21 by lsarraci          #+#    #+#             */
/*   Updated: 2025/12/16 18:20:26 by lsarraci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SHELL_H
# define SHELL_H

# include <unistd.h>
# include <stdlib.h>
# include <signal.h>
# include <sys/ioctl.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "../lib/libft/libft.h"
# include "../lib/ft_printf/ft_printf.h"
# include "builtins.h"
# include "display.h"
# include "lexer.h"
# include "parser.h"
# include "signals.h"
# include "structs.h"
# include "types.h"

# define F_BLACK "\033[30m"
# define F_RED "\033[31m"
# define F_GREEN "\033[32m"
# define F_YELLOW "\033[33m"
# define F_BLUE "\033[34m"
# define F_MAGENTA "\033[35m"
# define F_CYAN "\033[36m"
# define F_WHITE "\033[37m"

int		parser(char *input);
int		builtin_exit(char **args);

#endif
