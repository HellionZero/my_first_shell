/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsarraci <lsarraci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 15:30:00 by lsarraci          #+#    #+#             */
/*   Updated: 2026/01/27 16:43:57 by lsarraci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/shell.h"

int	is_valid_command(t_command *cmd)
{
	if (!cmd)
		return (0);
	if ((cmd->args && cmd->args[0]) || cmd->redirects)
		return (1);
	return (0);
}

t_command	*command_new(void)
{
	t_command	*cmd;

	cmd = malloc(sizeof(t_command));
	if (!cmd)
		return (NULL);
	cmd->args = NULL;
	cmd->redirects = NULL;
	cmd->next = NULL;
	return (cmd);
}

void	command_free(t_command *cmd)
{
	if (!cmd)
		return ;
	if (cmd->args)
		ft_free_split(cmd->args);
	if (cmd->redirects)
		redirect_list_free(cmd->redirects);
	free(cmd);
}

void	command_list_free(t_command *cmds)
{
	t_command	*tmp;

	while (cmds)
	{
		tmp = cmds->next;
		command_free(cmds);
		cmds = tmp;
	}
}

void	command_list_add_back(t_command **list, t_command *new)
{
	t_command	*current;

	if (!list || !new)
		return ;
	if (*list == NULL)
	{
		*list = new;
		return ;
	}
	current = *list;
	while (current->next)
		current = current->next;
	current->next = new;
}
