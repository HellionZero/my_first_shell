/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsarraci <lsarraci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/02 17:54:58 by lsarraci          #+#    #+#             */
/*   Updated: 2026/01/02 18:13:02 by lsarraci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/shell.h"

/*
** parse_env_string - Separa uma string "KEY=VALUE" em chave e valor
** 
** Parâmetros:
**   env_str: string no formato "KEY=VALUE"
**   key: ponteiro para armazenar a chave alocada
**   value: ponteiro para armazenar o valor alocado
**
** Retorna: 1 em sucesso, 0 em falha
*/
static int	parse_env_string(char *env_str, char **key, char **value)
{
	char	*equal_sign;
	size_t	key_len;

	if (!env_str)
		return (0);
	equal_sign = ft_strchr(env_str, '=');
	if (!equal_sign)
		return (0);
	key_len = equal_sign - env_str;
	*key = malloc(key_len + 1);
	if (!*key)
		return (0);
	ft_strlcpy(*key, env_str, key_len + 1);
	*value = ft_strdup(equal_sign + 1);
	if (!*value)
	{
		free(*key);
		return (0);
	}
	return (1);
}

/*
** increment_shlvl - Incrementa a variável SHLVL quando a shell inicia
**
** SHLVL indica o nível de profundidade da shell (quantas shells aninhadas)
** Se não existir, define como "1"
*/
static void	increment_shlvl(t_env *env)
{
	char	*shlvl_str;
	int		shlvl;
	char	*new_shlvl;

	shlvl_str = get_env_value(env, "SHLVL");
	if (shlvl_str)
	{
		shlvl = ft_atoi(shlvl_str);
		shlvl++;
	}
	else
		shlvl = 1;
	new_shlvl = ft_itoa(shlvl);
	if (new_shlvl)
	{
		env_set(env, "SHLVL", new_shlvl);
		free(new_shlvl);
	}
}

t_env	*init_env(char **envp)
{
	t_env		*env;
	char		*key;
	char		*value;
	int			i;

	env = malloc(sizeof(t_env));
	if (!env)
		return (NULL);
	env->vars = NULL;
	env->last_exit_status = 0;
	i = 0;
	while (envp && envp[i])
	{
		if (parse_env_string(envp[i], &key, &value))
		{
			env_set(env, key, value);
			free(key);
			free(value);
		}
		i++;
	}
	increment_shlvl(env);
	return (env);
}
