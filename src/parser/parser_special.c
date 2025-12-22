/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_special.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsarraci <lsarraci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/22 19:12:19 by lsarraci          #+#    #+#             */
/*   Updated: 2025/12/22 19:28:25 by lsarraci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/shell.h"

int	is_special_var(char *var_name)
{
	if (!var_name)
		return (0);
	if (ft_strncmp(var_name, "?", 2) == 0)
		return (1);
	if (ft_strncmp(var_name, "$", 2) == 0)
		return (1);
	if (ft_strncmp(var_name, "0", 2) == 0)
		return (1);
	return (0);
}

char	*expand_special_var(char *var_name)
{
	if (ft_strncmp(var_name, "?", 2) == 0)
		return (ft_itoa(get_exit_status()));
	if (ft_strncmp(var_name, "$", 2) == 0)
		return (ft_itoa(getpid()));
	if (ft_strncmp(var_name, "0", 2) == 0)
		return (ft_strdup("my_shell"));
	return (ft_strdup(""));
}
