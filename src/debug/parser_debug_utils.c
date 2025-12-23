/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_debug_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsarraci <lsarraci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/22 19:01:21 by lsarraci          #+#    #+#             */
/*   Updated: 2025/12/23 19:02:54 by lsarraci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/shell.h"

void	print_indent(int indent)
{
	int	i;

	i = 0;
	while (i++ < indent)
		ft_printf("  ");
}

void	print_args(char **args)
{
	int	i;

	i = 0;
	if (!args)
		return ;
	while (args[i])
	{
		ft_printf("%s", args[i]);
		if (args[i + 1])
			ft_printf(", ");
		i++;
	}
}

int	check_type(t_token_type type)
{
	if (type == TOKEN_REDIR_IN)
		return (1);
	else if (type == TOKEN_REDIR_OUT)
		return (2);
	else if (type == TOKEN_APPEND)
		return (3);
	else if (type == TOKEN_HEREDOC)
		return (4);
	return (0);
}

void	print_redirects(t_command *cmd)
{
	t_redirect	*redir;

	redir = cmd->redirects;
	while (redir)
	{
		if (redir->type == TOKEN_REDIR_IN)
			ft_printf(" < %s", redir->file);
		else if (redir->type == TOKEN_REDIR_OUT)
			ft_printf(" > %s", redir->file);
		else if (redir->type == TOKEN_APPEND)
			ft_printf(" >> %s", redir->file);
		else if (redir->type == TOKEN_HEREDOC)
			ft_printf(" << %s", redir->delimiter);
		redir = redir->next;
	}
}

void	print_command(t_command *cmd, int indent)
{
	print_indent(indent);
	ft_printf("Command: [");
	print_args(cmd->args);
	ft_printf("]\n");
	if (cmd->redirects)
	{
		print_indent(indent);
		ft_printf("Redirects:");
		print_redirects(cmd);
		ft_printf("\n");
	}
}
