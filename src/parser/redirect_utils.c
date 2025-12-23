/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsarraci <lsarraci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/23 16:39:40 by lsarraci          #+#    #+#             */
/*   Updated: 2025/12/23 16:39:53 by lsarraci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/shell.h"

t_redirect	*redirect_new(t_token_type type, char *file, char *delimiter)
{
	t_redirect	*redir;

	redir = malloc(sizeof(t_redirect));
	if (!redir)
		return (NULL);
	redir->type = type;
	redir->file = file;
	redir->delimiter = delimiter;
	redir->next = NULL;
	return (redir);
}

void	redirect_free(t_redirect *redir)
{
	if (!redir)
		return ;
	if (redir->file)
		free(redir->file);
	if (redir->delimiter)
		free(redir->delimiter);
	free(redir);
}

void	redirect_list_free(t_redirect *redirects)
{
	t_redirect	*tmp;

	while (redirects)
	{
		tmp = redirects->next;
		redirect_free(redirects);
		redirects = tmp;
	}
}

void	redirect_add_back(t_redirect **list, t_redirect *new)
{
	t_redirect	*current;

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
