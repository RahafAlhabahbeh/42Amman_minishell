/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaljazza <aaljazza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 07:12:10 by aaljazza          #+#    #+#             */
/*   Updated: 2025/07/09 07:12:53 by aaljazza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void call_env(t_minishell *minishell)
{
    printf("call env\n");
    int fd;
    
    fd = minishell->fd_out;
    if (fd == -1)
        fd = STDOUT_FILENO;

    int i;
    i = 0;
    while (environ[i] != NULL) {
        if (write(fd, environ[i], ft_strlen(environ[i])) == -1)
        {
            perror("write error");
            exit(1);
        }
        write(fd, "\n", 1);
        i++;
    }
}
