/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   call_exit.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rahaf <rahaf@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 08:53:38 by dal-mahr          #+#    #+#             */
/*   Updated: 2025/08/21 05:50:20 by rahaf            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

static void	exit_no_args(t_minishell *mini)
{
	if (!is_in_child_process(mini))
	{
		ft_putstr_fd("exit\n", STDERR_FILENO);
		rl_clear_history();
		free_minishell(mini);
	}
	else
	{
		if (mini->child_env)
			free_env_array_2(mini->child_env);
		cleanup_child_process(mini);
	}
	exit(mini->exit_status);
}

static void	exit_with_code(t_minishell *mini, char *arg)
{
	long long	code;

	if (!is_in_child_process(mini))
		ft_putstr_fd("exit\n", STDERR_FILENO);
	code = ft_atoi(arg);
	if (!is_in_child_process(mini))
	{
		rl_clear_history();
		free_minishell(mini);
	}
	else
	{
		if (mini->child_env)
			free_env_array_2(mini->child_env);
		cleanup_child_process(mini);
	}
	exit((unsigned char)code);
}

void	call_exit(t_minishell *mini, char **argv)
{
	if (!argv[1])
		return (exit_no_args(mini));
	if (!is_valid_exit_arg(argv[1]))
		handle_invalid_exit_arg(argv[1], mini);
	if (argv[2])
	{
		handle_too_many_args(mini);
		return ;
	}
	exit_with_code(mini, argv[1]);
}
/*
void	call_exit(t_minishell *mini, char **argv)
{
	long long	code;

	if (!is_in_child_process(mini))
		ft_putstr_fd("exit\n", STDERR_FILENO);
	if (!argv[1])
	{
		if (!is_in_child_process(mini))
		{
			rl_clear_history();
			rl_cleanup_after_signal();
			rl_deprep_terminal();
			free_minishell(mini);
		}
		else
			cleanup_child_process(mini);
		exit(mini->exit_status);
	}
	if (!is_valid_exit_arg(argv[1]))
		handle_invalid_exit_arg(argv[1], mini);
	if (argv[2])
	{
		handle_too_many_args(mini);
		return ;
	}
	code = ft_atoi(argv[1]);
	if (!is_in_child_process(mini))
	{
		rl_clear_history();

		free_minishell(mini);
	}
	else
		cleanup_child_process(mini);
	exit((unsigned char)code);
}
*/
