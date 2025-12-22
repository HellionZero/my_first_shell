/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_debug_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsarraci <lsarraci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/22 19:01:21 by lsarraci          #+#    #+#             */
/*   Updated: 2025/12/22 19:02:14 by lsarraci         ###   ########.fr       */
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

void	print_redirects(t_command *cmd)
{
	if (cmd->infile)
		ft_printf(" < %s", cmd->infile);
	if (cmd->outfile)
	{
		if (cmd->append)
			ft_printf(" >> %s", cmd->outfile);
		else
			ft_printf(" > %s", cmd->outfile);
	}
	if (cmd->heredoc)
		ft_printf(" << (heredoc)");
}

void	print_command(t_command *cmd, int indent)
{
	print_indent(indent);
	ft_printf("Command: [");
	print_args(cmd->args);
	ft_printf("]\n");
	if (cmd->infile || cmd->outfile || cmd->heredoc)
	{
		print_indent(indent);
		ft_printf("Redirects:");
		print_redirects(cmd);
		ft_printf("\n");
	}
}
