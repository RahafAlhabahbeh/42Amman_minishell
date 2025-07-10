/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_compare1.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaljazza <aaljazza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 07:20:33 by aaljazza          #+#    #+#             */
/*   Updated: 2025/07/09 07:21:08 by aaljazza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void redir_compare1(t_minishell *minishell)
{
	if (!strcmp(minishell->tok[minishell->i], "<"))
	{
		minishell->fd_in = open(minishell->tok[minishell->i + 1], O_RDONLY);
		if (minishell->fd_in == -1)
			perror("ERROR\n");
		minishell->i += 2;
	}
	else if (!strcmp(minishell->tok[minishell->i], ">"))
	{
		minishell->fd_out = open(minishell->tok[minishell->i + 1], O_RDWR | O_CREAT | O_TRUNC, 0777);
		if (minishell->fd_out == -1)
			perror("ERROR\n");
		minishell->i += 2;
	}
	else
		redir_compare2(minishell);
}