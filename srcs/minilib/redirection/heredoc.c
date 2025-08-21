/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rahaf <rahaf@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 01:11:13 by rahaf             #+#    #+#             */
/*   Updated: 2025/08/21 11:30:23 by rahaf            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"
#include <fcntl.h>

extern int	check_sigint_received(void);

void	flush_stdin_after_heredocs(void)
{
	int		flags;
	char	buffer[1024];

	flags = fcntl(STDIN_FILENO, F_GETFL);
	if (flags == -1)
		return ;
	if (fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK) == -1)
		return ;
	while (read(STDIN_FILENO, buffer, sizeof(buffer)) > 0)
		;
	fcntl(STDIN_FILENO, F_SETFL, flags);
	rl_clear_pending_input();
}
