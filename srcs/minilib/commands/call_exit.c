/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   call_exit.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dal-mahr <dal-mahr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 08:53:38 by dal-mahr          #+#    #+#             */
/*   Updated: 2025/07/27 14:16:30 by dal-mahr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

int	is_numeric(const char *str)
{
	int i = 0;

	if (!str || str[0] == '\0')
		return (0);
	if (str[i] == '-' || str[i] == '+')
		i++;
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

static int is_valid_exit_arg(const char *str)
{
	if (!str || str[0] == '\0')
		return (0);
	
	// Check for multiple + or - signs
	int sign_count = 0;
	int i = 0;
	
	while (str[i] == '+' || str[i] == '-')
	{
		sign_count++;
		i++;
	}
	
	if (sign_count > 1)
		return (0);
	
	// Check if rest is numeric
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	
	return (1);
}

void	call_exit(t_minishell *mini, char **argv)
{
	long long	code;

	ft_putstr_fd("exit\n", STDERR_FILENO);
	if (!argv[1])
		exit(mini->exit_status);
	
	// Check for too many arguments first
	if (argv[2])
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", STDERR_FILENO);
		mini->exit_status = 1;
		return;
	}
	
	// Check if argument is valid
	if (!is_valid_exit_arg(argv[1]))
	{
		ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
		ft_putstr_fd(argv[1], STDERR_FILENO);
		ft_putstr_fd(": numeric argument required\n", STDERR_FILENO);
		mini->exit_status = 2;
		return;
	}
	
	// Convert to number
	code = ft_atoi(argv[1]);
	
	// Check for overflow (bash limits to 8-bit unsigned)
	if (code > 255)
		code = 255;
	else if (code < 0)
		code = 0;
	
	exit((unsigned char)code);
}
