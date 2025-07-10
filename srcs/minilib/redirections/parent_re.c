/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parent_re.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaljazza <aaljazza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 07:19:50 by aaljazza          #+#    #+#             */
/*   Updated: 2025/07/09 07:19:51 by aaljazza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void parent_re(t_minishell *minishell)
{
	if (minishell->fd_app > 0)
	{
		close(minishell->fd_app);
		minishell->fd_app = -1;
	}
	if (minishell->fd_in > 0)
	{
		close(minishell->fd_in);
		minishell->fd_in = -1;
	}
	if (minishell->fd_out > 0)
	{
		close(minishell->fd_out);
		minishell->fd_out = -1;
	}
}
