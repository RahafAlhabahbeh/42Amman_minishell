/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   call_echo.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dal-mahr <dal-mahr@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 08:54:09 by dal-mahr          #+#    #+#             */
/*   Updated: 2025/07/31 01:16:40 by dal-mahr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

// void call_echo(char **argv)
// {
//     int i = 1;
//     int newline = 1;

//     while (argv[i] && argv[i][0] == '-' && argv[i][1] == 'n')
//     {
//         int j = 2;
//         while (argv[i][j] == 'n') j++;
//         if (argv[i][j] == '\0')
//         {
//             newline = 0;
//             i++;
//         }
//         else
//             break;
//     }

//     while (argv[i])
//     {
//         printf("%s", argv[i]);
//         if (argv[i + 1])
//             printf(" ");
//         i++;
//     }

//     if (newline)
//         printf("\n");

// }
void call_echo(t_minishell *mini, char **argv)
{
    int i = 1;
    int newline = 1;

    while (argv[i] && argv[i][0] == '-' && argv[i][1] == 'n')
    {
        int j = 2;
        while (argv[i][j] == 'n') j++;
        if (argv[i][j] == '\0')
        {
            newline = 0;
            i++;
        }
        else
            break;
    }
    int fd = mini->fd_out;
    if(fd == -1)
        fd = 1;

    while (argv[i])
    {
        ft_putstr_fd(argv[i], fd);
        if (argv[i + 1])
            ft_putchar_fd(' ', fd);
        i++;
    }

    if (newline)
        ft_putchar_fd('\n', fd);
}
