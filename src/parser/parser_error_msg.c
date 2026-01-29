/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_error_msg.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsarraci <lsarraci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/05 14:13:47 by lsarraci          #+#    #+#             */
/*   Updated: 2026/01/26 16:07:14 by lsarraci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/shell.h"

void	parse_error_node(char *msg, t_ast_node *node)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(msg, STDERR_FILENO);
	ft_putstr_fd("\n", STDERR_FILENO);
	node_free(node);
	set_exit_status(2);
}

void	parse_error_command(char *msg, t_command *cmd)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(msg, STDERR_FILENO);
	ft_putstr_fd("\n", STDERR_FILENO);
	command_free(cmd);
	set_exit_status(2);
}

void	empty_command_error(t_command *cmd)
{
	set_exit_status(0);
	command_free(cmd);
}

int	ambiguous_redirect_error(char *target)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	if (target && target[0])
		ft_putstr_fd(target, STDERR_FILENO);
	ft_putstr_fd(": ambiguous redirect\n", STDERR_FILENO);
	set_exit_status(1);
	if (target)
		free(target);
	return (0);
}

int	heredoc_limiter_error(char *delimiter)
{
	ft_putstr_fd("minishell: syntax error: ", STDERR_FILENO);
	ft_putstr_fd("invalid heredoc delimiter\n", STDERR_FILENO);
	set_exit_status(2);
	if (delimiter)
		free(delimiter);
	return (0);
}
