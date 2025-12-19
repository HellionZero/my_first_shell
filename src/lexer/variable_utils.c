/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variable_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsarraci <lsarraci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/19 17:19:13 by lsarraci          #+#    #+#             */
/*   Updated: 2025/12/19 18:18:58 by lsarraci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/shell.h"

int	is_special_var_char(char c)
{
	return (c == '?' || c == '$' || c == '#'
		|| c == '!' || c == '@' || c == '*');
}

int	is_var_name_char(char c)
{
	return (ft_isalnum(c) || c == '_');
}
