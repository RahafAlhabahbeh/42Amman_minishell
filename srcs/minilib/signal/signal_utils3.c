/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_utils3.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rahaf <rahaf@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 17:30:00 by dal-mahr          #+#    #+#             */
/*   Updated: 2025/08/20 16:07:32 by rahaf            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

int	is_in_child_process(void)
{
	return (g_in_child_process);
}

void	set_in_heredoc(int in_heredoc)
{
	g_in_heredoc = in_heredoc;
}

int	is_in_heredoc(void)
{
	return (g_in_heredoc);
}

void	setup_signal_handling(void)
{
	struct sigaction	sa;

	sa.sa_handler = SIG_IGN;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, NULL);
}
