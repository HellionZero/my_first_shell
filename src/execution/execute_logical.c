/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_logical.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsarraci <lsarraci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/02 14:57:22 by lsarraci          #+#    #+#             */
/*   Updated: 2026/01/29 15:07:18 by lsarraci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/shell.h"

int	execute_logical_and(t_ast_node *left, t_ast_node *right, t_env *env)
{
	int	left_status;

	if (!left || !right)
		return (1);
	setup_signals_executing();
	left_status = execute_ast(left, env);
	setup_signals_interactive();
	if (left_status == 0)
	{
		setup_signals_executing();
		left_status = execute_ast(right, env);
		setup_signals_interactive();
		return (left_status);
	}
	return (left_status);
}

int	execute_logical_or(t_ast_node *left, t_ast_node *right, t_env *env)
{
	int	left_status;

	if (!left || !right)
		return (1);
	setup_signals_executing();
	left_status = execute_ast(left, env);
	setup_signals_interactive();
	if (left_status != 0)
	{
		setup_signals_executing();
		left_status = execute_ast(right, env);
		setup_signals_interactive();
		return (left_status);
	}
	return (left_status);
}
