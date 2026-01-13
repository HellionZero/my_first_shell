/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_expansion.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: loda-sil <loda-sil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/22 17:19:18 by lsarraci          #+#    #+#             */
/*   Updated: 2026/01/12 02:31:19 by loda-sil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/shell.h"

char	*expand_variable(char *var_name)
{
	t_env_var	*env_var;
	t_env		*env;

	if (!var_name)
		return (ft_strdup(""));
	if (is_special_var(var_name))
		return (expand_special_var(var_name));
	env = get_shell_env();
	if (!env)
		return (ft_strdup(""));
	env_var = env_get(env, var_name);
	if (!env_var)
		return (ft_strdup(""));
	return (ft_strdup(env_var->value));
}

static char	*process_quoted_variable(char *str, int *i)
{
	char	*var_value;
	char	*var_name;
	int		start;

	start = ++(*i);
	while (str[*i] && (ft_isalnum(str[*i]) || str[*i] == '_'
			|| str[*i] == '?' || str[*i] == '$'))
		(*i)++;
	var_name = ft_substr(str, start, *i - start);
	var_value = expand_variable(var_name);
	free(var_name);
	return (var_value);
}

char	*expand_in_double_quotes(char *str)
{
	char	*result;
	char	*var_value;
	int		i;

	result = ft_strdup("");
	i = 0;
	while (str[i])
	{
		if (str[i] == '$' && str[i + 1] && (ft_isalnum(str[i + 1])
				|| str[i + 1] == '_' || str[i + 1] == '?'))
		{
			var_value = process_quoted_variable(str, &i);
			result = join_and_free(result, var_value);
			free(var_value);
		}
		else
		{
			result = join_char_and_free(result, str[i]);
			i++;
		}
	}
	return (result);
}

char	*expand_word(t_word_part *parts)
{
	char		*result;
	char		*expanded;

	result = ft_strdup("");
	while (parts)
	{
		if (parts->type == PART_LITERAL || parts->type == PART_SINGLE_QUOTE)
			result = join_and_free(result, parts->content);
		else if (parts->type == PART_DOUBLE_QUOTE)
		{
			expanded = expand_in_double_quotes(parts->content);
			result = join_and_free(result, expanded);
			free(expanded);
		}
		else if (parts->type == PART_VARIABLE)
		{
			expanded = expand_variable(parts->content);
			result = join_and_free(result, expanded);
			free(expanded);
		}
		parts = parts->next;
	}
	return (result);
}
