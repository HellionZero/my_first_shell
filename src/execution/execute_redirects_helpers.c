/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_redirects_helpers.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsarraci <lsarraci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/09 19:57:45 by lsarraci          #+#    #+#             */
/*   Updated: 2026/01/27 19:01:56 by lsarraci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/shell.h"

int	is_env_builtin(char *command)
{
	if (ft_strcmp(command, "cd") == 0
		|| ft_strcmp(command, "export") == 0
		|| ft_strcmp(command, "unset") == 0
		|| ft_strcmp(command, "exit") == 0)
		return (1);
	return (0);
}

int	close_and_exit(int fd_in, int fd_out, int ex_status)
{
	if (fd_in >= 0)
		dup2(fd_in, 0);
	if (fd_out >= 0)
		dup2(fd_out, 1);
	if (fd_in >= 0)
		close(fd_in);
	if (fd_out >= 0)
		close(fd_out);
	return (ex_status);
}

void	resolve_fd(int	*fd)
{
	if (fd && *fd != -1 && *fd != STDIN_FILENO)
	{
		dup2(*fd, STDIN_FILENO);
		close(*fd);
	}
}

int	decide_command_type(char **args, t_env *env)
{
	int	result;

	if (is_builtin(args[0]))
	{
		if (is_env_builtin(args[0]))
			result = execute_builtin(args, env);
		else
			result = execute_builtin_files(args, env);
	}
	else
		result = execute_external_command(args, env);
	return (result);
}

int	is_redirect_needed(t_command *cmd)
{
	t_redirect	*current;

	if (!cmd || !cmd->redirects)
		return (0);
	current = cmd->redirects;
	while (current)
	{
		if ((current->type == TOKEN_REDIR_IN
				|| current->type == TOKEN_REDIR_OUT
				|| current->type == TOKEN_APPEND
				|| current->type == TOKEN_HEREDOC)
			&& current->file != NULL)
			return (1);
		current = current->next;
	}
	return (0);
}
