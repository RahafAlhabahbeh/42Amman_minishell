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
	int fd;
	
    	fd = minishell->fd_out;
    	if (fd == -1)
		fd = 1;
	if (getcwd(minishell->buff, sizeof(minishell->buff)) != NULL) 
	{
		write(fd, minishell->buff, ft_strlen(minishell->buff));
		write(fd, "\n", 1);
	}
	else
	{
                perror("getcwd() error");
        }
}
