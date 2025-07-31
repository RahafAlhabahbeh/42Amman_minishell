/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   call_pwd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dal-mahr <dal-mahr@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 08:54:52 by dal-mahr          #+#    #+#             */
/*   Updated: 2025/07/31 01:15:40 by dal-mahr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"
#include <limits.h>

// void call_pwd(void)
// {
//     char cwd[PATH_MAX];

//     if (getcwd(cwd, sizeof(cwd)) != NULL)
//         printf("%s\n", cwd);
//     else
//         perror("pwd");
// }

void call_pwd(t_minishell *mini)
{
    char cwd[PATH_MAX];
    int fd = mini->fd_out;
    if (fd == -1)
        fd = 1;

    if (getcwd(cwd, sizeof(cwd)) != NULL)
    {
        ft_putstr_fd(cwd, fd);
        ft_putchar_fd('\n', fd);
    }
    else
        perror("pwd");
}

