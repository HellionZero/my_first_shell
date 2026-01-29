/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_init.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsarraci <lsarraci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/06 19:39:01 by lsarraci          #+#    #+#             */
/*   Updated: 2026/01/29 15:08:46 by lsarraci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/shell.h"

t_env	*init_shell(char **envp)
{
	t_env	*env;
	int		ctty_fd;

	env = init_env(envp);
	if (!env)
		return (NULL);
	ctty_fd = open("/dev/tty", O_RDONLY);
	if (getenv("NO_COLOR") == NULL)
	{
		if (isatty(STDIN_FILENO) || isatty(STDOUT_FILENO)
			|| ctty_fd != -1)
		{
			if (ctty_fd != -1)
				close(ctty_fd);
			display_banner();
		}
		else if (ctty_fd != -1)
			close(ctty_fd);
	}
	else if (ctty_fd != -1)
		close(ctty_fd);
	return (env);
}

void	cleanup_shell(t_env *env)
{
	if (!env)
		return ;
	free_env(env);
}

void	shell_cleanup_and_exit(t_env *env, int exit_code)
{
	rl_clear_history();
	cleanup_shell(env);
	exit(exit_code);
}
