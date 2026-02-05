/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsarraci <lsarraci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/02 17:50:56 by lsarraci          #+#    #+#             */
/*   Updated: 2026/02/02 16:52:43 by lsarraci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENV_H
# define ENV_H

# include "structs.h"

/*--------- environment initialization -----------------*/

/* function to initialize the environment structure 
from an array of strings, which is 
the envp parameter from main, and set the current shell level. */
t_env		*init_env(char **envp);

/*function to free the environment structure */
void		free_env(t_env *env);

/*--------- environment management utils -----------------*/

/*function to create a new environment variable node */
t_env_var	*create_env_node(char *key, char *value);

/*function to add a new environment variable node to the end of the list */
void		add_env_node_to_end(t_env *env, t_env_var *node);

/*function to free a single environment variable node */
void		free_env_node(t_env_var *node);

/*function to free a linked list of environment variable nodes */
void		free_env_list(t_env_var *list);

/*function to create a deep copy of the environment structure to 
be used inside a sequence of pipes, operators and subshells */
t_env		*deep_copy_env(t_env *src);

/*--------- environment variable operations -------------*/

/*function to get an environment variable node by key */
t_env_var	*env_get(t_env *env, char *key);

/*function to set an environment variable based on key and value */
void		env_set(t_env *env, char *key, char *value);

/*function to unset an environment variable based on key */
void		env_unset(t_env *env, char *key);

/*function to get the value of an environment variable by key */
char		*get_env_value(t_env *env, char *key);

/*--------- environment conversion -----------------*/

/*function to convert the environment structure
to an array of strings, based of the number of variables and 
strings inside the current environment */
char		**env_to_array(t_env *env);

/*function to free an array of environment strings */
void		free_env_array(char **envp);

#endif