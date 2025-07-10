/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_re.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaljazza <aaljazza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 07:18:20 by aaljazza          #+#    #+#             */
/*   Updated: 2025/07/09 07:18:46 by aaljazza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "minishell.h"

void child_re(t_minishell *minishell)
{
	if (minishell->fd_in > 0)
	{
		if (dup2(minishell->fd_in, -1) == -1)
		{
			perror("ERROR\n");
		}
		close(minishell->fd_in);
	}
	if (minishell->fd_out > 0)
	{
		dup2(minishell->fd_out, 1);
		close(minishell->fd_out);
	}
	if (minishell->fd_app > 0)
	{
		dup2(minishell->fd_app, 1);
		close(minishell->fd_app);
	}
}