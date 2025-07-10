/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   call_echo.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaljazza <aaljazza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 07:09:34 by aaljazza          #+#    #+#             */
/*   Updated: 2025/07/09 07:10:12 by aaljazza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void call_echo(t_minishell *minishell, int op)
{
    int fd;
    int i;
	
    fd = minishell->fd_out;
    if (fd == -1)
	fd = 1;
    i = 1;
    if (op)
    	i++;
    while (minishell->cmd[i])
    {
	    if (write(fd, minishell->cmd[i], ft_strlen(minishell->cmd[i])) == -1)
	    {
	    	printf("error\n");
		perror("ECHO error\n");
		exit(1);
	    }
	    i++;
	    if (!minishell->cmd[i])
	    	break;
	    write(fd, " ", 1);
    }
    if (!op)
        write(fd, "\n", 1);
}