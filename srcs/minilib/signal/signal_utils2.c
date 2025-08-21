/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_utils2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rahaf <rahaf@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 17:30:00 by dal-mahr          #+#    #+#             */
/*   Updated: 2025/08/21 02:35:57 by rahaf            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

void	reset_received_signal(void)
{
	g_received_signal = 0;
}

void	set_child_running(t_minishell *mini, int running)
{
	if (mini)
		mini->child_running = running;
}

int	is_child_running(t_minishell *mini)
{
	if (!mini)
		return (0);
	return (mini->child_running);
}

void	set_in_child_process(t_minishell *mini, int in_child)
{
	if (mini)
		mini->in_child_process = in_child;
}
