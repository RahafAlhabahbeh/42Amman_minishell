/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_utils3.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rahaf <rahaf@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 17:30:00 by dal-mahr          #+#    #+#             */
/*   Updated: 2025/08/21 02:35:57 by rahaf            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

int	is_in_child_process(t_minishell *mini)
{
	if (!mini)
		return (0);
	return (mini->in_child_process);
}

void	set_in_heredoc(t_minishell *mini, int in_heredoc)
{
	if (mini)
		mini->in_heredoc = in_heredoc;
}

int	is_in_heredoc(t_minishell *mini)
{
	if (!mini)
		return (0);
	return (mini->in_heredoc);
}

void	setup_signal_handling(void)
{
	struct sigaction	sa;

	sa.sa_handler = SIG_IGN;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, NULL);
}
