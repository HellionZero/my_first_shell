/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsarraci <lsarraci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/02 17:50:56 by lsarraci          #+#    #+#             */
/*   Updated: 2026/01/27 15:50:35 by lsarraci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENV_H
# define ENV_H

# include "structs.h"

/*--------- environment initialization -----------------*/
t_env		*init_env(char **envp);
void		free_env(t_env *env);

/*--------- environment management utils -----------------*/
t_env_var	*create_env_node(char *key, char *value);
void		free_env_node(t_env_var *node);
void		free_env_list(t_env_var *list);
t_env		*deep_copy_env(t_env *src);

t_env_var	*env_get(t_env *env, char *key);
void		env_set(t_env *env, char *key, char *value);
void		env_unset(t_env *env, char *key);
char		*get_env_value(t_env *env, char *key);
void		add_env_node_to_end(t_env *env, t_env_var *node);

/*--------- environment conversion -----------------*/
char		**env_to_array(t_env *env);
void		free_env_array(char **envp);

#endif