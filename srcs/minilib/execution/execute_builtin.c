/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_builtin.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dal-mahr <dal-mahr@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 08:55:21 by dal-mahr          #+#    #+#             */
/*   Updated: 2025/07/24 08:56:51 by dal-mahr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

int is_builtin(char *cmd)
{
    if (!cmd)
        return 0;
    return (!strcmp(cmd, "echo") || !strcmp(cmd, "cd") || !strcmp(cmd, "pwd") ||
            !strcmp(cmd, "env") || !strcmp(cmd, "export") || !strcmp(cmd, "unset") ||
            !strcmp(cmd, "exit"));
}

void execute_builtin(t_minishell *minishell, int i)
{
    char *cmd = minishell->cmd[i].argv[0];

    if (strcmp(minishell->cmd[i].argv[0], "echo") == 0)
        call_echo(minishell->cmd[i].argv);
    else if (!strcmp(cmd, "pwd"))
        call_pwd();
    else if (!strcmp(cmd, "env"))
        call_env(minishell);
    else if (!strcmp(cmd, "export"))
        call_export(minishell, minishell->cmd[i].argv);
    else if (!strcmp(cmd, "unset"))
        call_unset(minishell, minishell->cmd[i].argv);
    else if (!strcmp(cmd, "env")) 
        call_env(minishell);
    else if (!strcmp(cmd, "unset")) 
        call_unset(minishell, minishell->cmd[i].argv);
    else if (!strcmp(cmd, "exit"))
        call_exit(minishell, minishell->cmd[i].argv);
    else if (!strcmp(cmd, "cd"))
        call_cd(minishell, minishell->cmd[i].argv);    
    minishell->exit_status = 0;
}
