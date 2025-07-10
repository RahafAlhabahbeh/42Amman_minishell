/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   compare_commands.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaljazza <aaljazza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 00:20:47 by aaljazza          #+#    #+#             */
/*   Updated: 2025/07/09 07:32:09 by aaljazza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void compare_commands (t_minishell *minishell)
{
        if (!strcmp(minishell->cmd[0], "env"))
            call_env(minishell);
        if (!strcmp(minishell->cmd[0], "echo"))
        {
            if (!strcmp(minishell->cmd[1], "-n"))
                call_echo(minishell, 1);
            else
                call_echo(minishell, 0);
        }
        else if (!strcmp(minishell->cmd[0], "pwd"))
            call_pwd(minishell);
        else
        {
            execvp(minishell->cmd[0], minishell->cmd);
            perror("execvp");
            exit(1);
        }
}