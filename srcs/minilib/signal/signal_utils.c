/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dal-mahr <dal-mahr@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 17:30:00 by dal-mahr          #+#    #+#             */
/*   Updated: 2025/08/12 17:30:00 by dal-mahr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

static volatile sig_atomic_t	g_child_running = 0;
static volatile sig_atomic_t	g_in_child_process = 0;
extern volatile sig_atomic_t	g_received_signal;

int	check_sigint_received(void)
{
	if (g_received_signal == SIGINT)
	{
		g_received_signal = 0;
		return (1);
	}
	return (0);
}

int	peek_sigint_received(void)
{
	return (g_received_signal == SIGINT);
}

int	check_sigquit_received(void)
{
	if (g_received_signal == SIGQUIT)
	{
		g_received_signal = 0;
		return (1);
	}
	return (0);
}

int	get_received_signal(void)
{
	return (g_received_signal);
}

void	reset_received_signal(void)
{
	g_received_signal = 0;
}

void	set_child_running(int running)
{
	if (running)
		g_child_running = 1;
	else
		g_child_running = 0;
}

int	is_child_running(void)
{
	return (g_child_running);
}

void	set_in_child_process(int in_child)
{
	g_in_child_process = in_child;
}

int	is_in_child_process(void)
{
	return (g_in_child_process);
}
