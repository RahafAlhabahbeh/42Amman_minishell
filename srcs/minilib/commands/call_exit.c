/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   call_exit.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dal-mahr <dal-mahr@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 08:53:38 by dal-mahr          #+#    #+#             */
/*   Updated: 2025/08/12 17:30:00 by dal-mahr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

int	is_numeric(const char *str)
{
	int	i;

	i = 0;
	if (!str || str[0] == '\0')
		return (0);
	if (str[i] == '-' || str[i] == '+')
		i++;
	if (!str[i])
		return (0);
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

static int	is_valid_exit_arg(const char *str)
{
	int	sign_count;
	int	i;

	if (!str || str[0] == '\0')
		return (0);
	sign_count = 0;
	i = 0;
	while (str[i] == '+' || str[i] == '-')
	{
		sign_count++;
		i++;
	}
	if (sign_count > 1)
		return (0);
	if (!str[i])
		return (0);
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

static void	handle_invalid_exit_arg(char *arg, t_minishell *mini)
{
	if (!is_in_child_process())
	{
		ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
		ft_putstr_fd(arg, STDERR_FILENO);
		ft_putstr_fd(": numeric argument required\n", STDERR_FILENO);
		rl_clear_history();
		rl_cleanup_after_signal();
		rl_deprep_terminal();
		free_minishell(mini);
	}
	else
		cleanup_child_process(mini);
	exit(2);
}

static void	handle_too_many_args(t_minishell *mini)
{
	if (!is_in_child_process())
		ft_putstr_fd("minishell: exit: too many arguments\n", STDERR_FILENO);
	if (is_in_child_process())
		exit(1);
	mini->exit_status = 1;
}

void	call_exit(t_minishell *mini, char **argv)
{
	long long	code;

	if (!is_in_child_process())
		ft_putstr_fd("exit\n", STDERR_FILENO);
	if (!argv[1])
	{
		if (!is_in_child_process())
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
	if (!is_in_child_process())
	{
		rl_clear_history();
		rl_cleanup_after_signal();
		rl_deprep_terminal();
		free_minishell(mini);
	}
	else
		cleanup_child_process(mini);
	exit((unsigned char)code);
}
