/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rahaf <rahaf@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 17:30:00 by dal-mahr          #+#    #+#             */
/*   Updated: 2025/08/17 16:38:46 by rahaf            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

volatile sig_atomic_t	g_child_running = 0;
volatile sig_atomic_t	g_in_child_process = 0;

int	check_sigint_received(void)
{
	if (g_received_signal == 130)
	{
		g_received_signal = 0;
		return (1);
	}
	return (0);
}

int	peek_sigint_received(void)
{
	return (g_received_signal == 130);
}

int	check_sigquit_received(void)
{
	if (g_received_signal == 131)
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
