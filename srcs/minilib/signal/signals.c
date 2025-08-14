/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dal-mahr <dal-mahr@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 08:55:21 by dal-mahr          #+#    #+#             */
/*   Updated: 2025/08/12 17:30:00 by dal-mahr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

volatile sig_atomic_t	g_received_signal = 0;

void	handle_sigint(int sig)
{
	g_received_signal = sig;
	if (!is_child_running())
	{
		write(1, "^C\n", 3);
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
	}
	g_received_signal = 130;
}

void	handle_sigquit(int sig)
{
	g_received_signal = sig;
}

void	setup_signals(void)
{
	struct sigaction	sa;
	//extern int			rl_catch_signals;

	sa.sa_handler = handle_sigint;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, NULL);
	sa.sa_handler = handle_sigquit;
	sigaction(SIGQUIT, &sa, NULL);
	sa.sa_handler = SIG_IGN; // ignore SIGQUIT
	sigaction(SIGQUIT, &sa, NULL);
	//rl_catch_signals = 0;
}

void	set_minishell_pointer(t_minishell *mini)
{
	(void)mini;
}

void	child_sig()
{
	struct sigaction sa;

    sa.sa_handler = SIG_DFL;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    sigaction(SIGINT, &sa, NULL);
	struct sigaction sas;

    sas.sa_handler = SIG_DFL;
    sigemptyset(&sas.sa_mask);
    sas.sa_flags = SA_RESTART;
    sigaction(SIGQUIT, &sas, NULL);
	
}
