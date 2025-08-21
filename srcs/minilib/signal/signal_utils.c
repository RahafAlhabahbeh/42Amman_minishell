/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rahaf <rahaf@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 17:30:00 by dal-mahr          #+#    #+#             */
/*   Updated: 2025/08/21 02:49:55 by rahaf            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"



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
