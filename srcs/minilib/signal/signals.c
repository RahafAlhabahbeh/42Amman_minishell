/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rahaf <rahaf@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 08:55:21 by dal-mahr          #+#    #+#             */
/*   Updated: 2025/08/18 12:10:06 by rahaf            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

volatile sig_atomic_t	g_received_signal = 0;

void	handle_sigint(int sig)
{
	if (!is_child_running() && !is_in_heredoc())
	{
		g_received_signal = 130;
		write(1, "\n", 1);
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
	}
	else
	{
		g_received_signal = sig;
	}
}

void	handle_sigquit(int sig)
{
	if (!is_child_running())
	{
		return ;
	}
	else
	{
		g_received_signal = sig;
	}
}

void	setup_signals(void)
{
	struct sigaction	sa;

	sa.sa_handler = handle_sigint;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, NULL);
	sa.sa_handler = SIG_IGN;
	sigaction(SIGQUIT, &sa, NULL);
}

void	set_minishell_pointer(t_minishell *mini)
{
	(void)mini;
}

void	child_sig(void)
{
	struct sigaction	sa;
	struct sigaction	sas;

	sa.sa_handler = SIG_DFL;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sa, NULL);
	sas.sa_handler = SIG_DFL;
	sigemptyset(&sas.sa_mask);
	sas.sa_flags = SA_RESTART;
	sigaction(SIGQUIT, &sas, NULL);
}
