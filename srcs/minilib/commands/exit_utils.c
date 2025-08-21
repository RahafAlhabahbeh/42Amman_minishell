/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rahaf <rahaf@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 21:00:00 by dal-mahr          #+#    #+#             */
/*   Updated: 2025/08/21 02:35:57 by rahaf            ###   ########.fr       */
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

int	is_valid_exit_arg(const char *str)
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

void	handle_invalid_exit_arg(char *arg, t_minishell *mini)
{
	if (!is_in_child_process(mini))
	{
		ft_putstr_fd("exit\n", STDERR_FILENO);
		ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
		ft_putstr_fd(arg, STDERR_FILENO);
		ft_putstr_fd(": numeric argument required\n", STDERR_FILENO);
		rl_clear_history();

		free_minishell(mini);
	}
	else
	{
		if (mini->child_env)
			free_env_array_2(mini->child_env);
		cleanup_child_process(mini);
	}
	exit(2);
}

void	handle_too_many_args(t_minishell *mini)
{
	if (!is_in_child_process(mini))
		ft_putstr_fd("minishell: exit: too many arguments\n", STDERR_FILENO);
	if (is_in_child_process(mini))
	{
		if (mini->child_env)
			free_env_array_2(mini->child_env);
		cleanup_child_process(mini);
		exit(1);
	}
	mini->exit_status = 1;
}
