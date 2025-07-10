/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_fork.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaljazza <aaljazza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 22:14:18 by aaljazza          #+#    #+#             */
/*   Updated: 2025/07/09 07:24:47 by aaljazza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Fork a child process in the main.
// Redirection in child.
// Compare the command in commands array to execute them
// Redirection in child.

void main_fork(t_minishell *minishell, int pid)
{
    if (pid == 0)
    {
        child_re(minishell);
        compare_commands (minishell);
    }
    else
    {
        parent_re(minishell);
        int status;
        waitpid(pid, &status, 0);
    }
}

