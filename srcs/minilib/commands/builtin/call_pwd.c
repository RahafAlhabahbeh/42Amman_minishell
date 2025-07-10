/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cal_pwd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaljazza <aaljazza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 07:10:45 by aaljazza          #+#    #+#             */
/*   Updated: 2025/07/09 07:11:30 by aaljazza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void call_pwd(t_minishell *minishell)
{
	printf("call pwd\n");
	if (getcwd(minishell->buff, sizeof(minishell->buff)) != NULL) 
	{
		if (minishell->fd_out == -1)
			minishell->fd_out = STDOUT_FILENO;
		write(minishell->fd_out, minishell->buff, ft_strlen(minishell->buff));
		write(minishell->fd_out, "\n", 1);
	}
	else
	{
                perror("getcwd() error");
        }
}