/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_command_helpers.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsarraci <lsarraci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/29 15:41:46 by lsarraci          #+#    #+#             */
/*   Updated: 2026/01/29 15:58:51 by lsarraci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/shell.h"

static int	create_out_or_append(t_redirect *r)
{
	int	flags;
	int	fd;

	flags = O_WRONLY | O_CREAT;
	if (r->type == TOKEN_APPEND)
		flags |= O_APPEND;
	else
		flags |= O_TRUNC;
	fd = open(r->file, flags, 0644);
	if (fd >= 0)
		close(fd);
	(void)fd;
	return (1);
}

static int	create_in_file(t_redirect *r)
{
	int	fd;

	fd = open(r->file, O_RDONLY);
	if (fd >= 0)
		close(fd);
	(void)fd;
	return (1);
}

static void	handle_heredoc_close(t_redirect *r)
{
	if (r->heredoc_fd >= 0)
		close(r->heredoc_fd);
	r->heredoc_fd = -1;
}

int	prepare_and_spawn_external(t_command *cmd, t_env *env)
{
	char	*executable;
	int		command_code;

	executable = get_command_executable(cmd, env);
	command_code = set_command_code(cmd, &executable);
	if (command_code != 1)
		return (command_code);
	if (is_directory_error(executable) == 126)
	{
		free(executable);
		return (126);
	}
	return (fork_and_execute(cmd, executable, env));
}

void	ensure_redirect_files_created(t_command *cmd)
{
	t_redirect	*r;

	if (!cmd || (cmd && cmd->args && cmd->args[0] && cmd->args[0][0] != '\0'))
		return ;
	if (!cmd || !cmd->redirects)
		r = NULL;
	else
		r = cmd->redirects;
	while (r)
	{
		if (r->type == TOKEN_REDIR_OUT || r->type == TOKEN_APPEND)
			create_out_or_append(r);
		else if (r->type == TOKEN_REDIR_IN)
			create_in_file(r);
		else if (r->type == TOKEN_HEREDOC)
			handle_heredoc_close(r);
		r = r->next;
	}
}
