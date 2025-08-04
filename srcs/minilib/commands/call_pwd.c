/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   call_pwd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dal-mahr <dal-mahr@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 08:54:52 by dal-mahr          #+#    #+#             */
/*   Updated: 2025/08/04 09:34:37 by dal-mahr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"
#include <limits.h>

void call_pwd(t_minishell *mini __attribute__((unused)))
{
    char cwd[PATH_MAX];

    if (getcwd(cwd, sizeof(cwd)) != NULL)
    {
        ft_putstr_fd(cwd, STDOUT_FILENO);
        ft_putchar_fd('\n', STDOUT_FILENO);
    }
    else
        perror("pwd");
}

