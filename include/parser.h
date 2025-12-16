/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsarraci <lsarraci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 15:11:15 by lsarraci          #+#    #+#             */
/*   Updated: 2025/12/16 15:11:29 by lsarraci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include "structs.h"

/* Command creation and manipulation */
t_command	*command_new(void);
void		command_free(t_command *cmd);
void		command_list_free(t_command *cmds);
void		command_list_add_back(t_command **list, t_command *new);

#endif