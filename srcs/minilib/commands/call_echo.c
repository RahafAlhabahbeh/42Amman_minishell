/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   call_echo.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dal-mahr <dal-mahr@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 08:54:09 by dal-mahr          #+#    #+#             */
/*   Updated: 2025/08/04 09:26:25 by dal-mahr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

void call_echo(t_minishell *mini, char **argv)
{
    int i = 1;
    int newline = 1;

    // Handle -n flag(s)
    while (argv[i] && argv[i][0] == '-' && argv[i][1] == 'n')
    {
        int j = 2;
        while (argv[i][j] == 'n')
            j++;
        if (argv[i][j] == '\0')
        {
            newline = 0;
            i++;
        }
        else
            break;
    }

    // Print arguments separated by spaces
    while (argv[i])
    {
        ft_putstr_fd(argv[i], STDOUT_FILENO);
        if (argv[i + 1])
            ft_putchar_fd(' ', STDOUT_FILENO);
        i++;
    }

    // Print newline unless -n flag was used
    if (newline)
        ft_putchar_fd('\n', STDOUT_FILENO);
    
    mini->exit_status = 0;
}
