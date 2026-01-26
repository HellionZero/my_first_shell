/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsarraci <lsarraci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/09 16:22:34 by lsarraci          #+#    #+#             */
/*   Updated: 2026/01/26 17:57:42 by lsarraci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/shell.h"

static void	process_input(char *input, t_env *env)
{
	t_token		*tokens;
	t_ast_node	*tree;
	int			status;

	tokens = lexer(input, 0);
	if (!tokens)
		return ;
	if (is_debug_enabled())
		print_tokens(tokens);
	tree = parse_tokens(tokens);
	if (!tree)
	{
		token_list_free(tokens);
		return ;
	}
	if (is_debug_enabled())
		print_ast(tree);
	status = execute_ast(tree, env);
	if (!env->should_exit)
		env->exit_code = status;
	set_exit_status(env->exit_code);
	node_free(tree);
	token_list_free(tokens);
}

static int	process_line(char *input, t_env *env, int is_interactive)
{
	if (!input[0])
		return (0);
	if (is_interactive)
		add_history(input);
	process_input(input, env);
	handle_signal_after_execution();
	ensure_newline_for_prompt();
	return (env->should_exit);
}

static void	shell_loop(t_env *env)
{
	char	*input;
	int		is_interactive;

	is_interactive = isatty(STDIN_FILENO);
	while (1)
	{
		input = read_line_with_prompt(build_prompt());
		if (should_exit_shell(input))
		{
			ft_printf("exit\n");
			break ;
		}
		handle_signal_after_readline(&input);
		if (input && process_line(input, env, is_interactive))
		{
			free(input);
			break ;
		}
		if (input)
			free(input);
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_env	*env;
	int		exit_code;

	(void)argc;
	(void)argv;
	env = init_shell(envp);
	if (!env)
		return (1);
	set_shell_env(env);
	setup_signals_interactive();
	shell_loop(env);
	exit_code = env->exit_code;
	rl_clear_history();
	cleanup_shell(env);
	reset_shell_env();
	set_exit_status(exit_code);
	ft_printf("[DEBUG] main: env->exit_code = %d\n", env->exit_code);
	return (env->exit_code);
}
