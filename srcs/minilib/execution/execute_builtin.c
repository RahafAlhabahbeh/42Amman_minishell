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

// void execute_builtin(t_minishell *minishell, int i)
// {
//     char *cmd = minishell->cmd[i].argv[0];

//     if (strcmp(minishell->cmd[i].argv[0], "echo") == 0)
//         call_echo(minishell->cmd[i].argv);
//     else if (!strcmp(cmd, "pwd"))
//         call_pwd();
//     else if (!strcmp(cmd, "env"))
//         call_env(minishell);
//     else if (!strcmp(cmd, "export"))
//         call_export(minishell, minishell->cmd[i].argv);
//     else if (!strcmp(cmd, "unset"))
//         call_unset(minishell, minishell->cmd[i].argv);
//     else if (!strcmp(cmd, "env")) 
//         call_env(minishell);
//     else if (!strcmp(cmd, "unset")) 
//         call_unset(minishell, minishell->cmd[i].argv);
//     else if (!strcmp(cmd, "exit"))
//         call_exit(minishell, minishell->cmd[i].argv);
//     else if (!strcmp(cmd, "cd"))
//         call_cd(minishell, minishell->cmd[i].argv);    
//     minishell->exit_status = 0;
// }

void execute_builtin(t_minishell *minishell, int i)
{
    t_cmd *cmd = minishell->cmd;
    int j = 0;

    // Traverse to the i-th command node
    while (cmd && j < i)
    {
        cmd = cmd->next;
        j++;
    }
    if (!cmd || !cmd->argv || !cmd->argv[0])
        return;

    char *name = cmd->argv[0];

    if (!ft_strcmp(name, "echo"))
        call_echo(cmd->argv);
    else if (!ft_strcmp(name, "pwd"))
        call_pwd();
    else if (!ft_strcmp(name, "env"))
        call_env(minishell);
    else if (!ft_strcmp(name, "export"))
        call_export(minishell, cmd->argv);
    else if (!ft_strcmp(name, "unset"))
        call_unset(minishell, cmd->argv);
    else if (!ft_strcmp(name, "exit"))
        call_exit(minishell, cmd->argv);
    else if (!ft_strcmp(name, "cd"))
        call_cd(minishell, cmd->argv);

    minishell->exit_status = 0;
}

int ft_strcmp(const char *s1, const char *s2)
{
    int i = 0;

    while (s1[i] && s2[i] && s1[i] == s2[i])
        i++;
    return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}


/*
Build in comman: [export + unset + cd + exit + echo + pwd + env]
* is execution on the parent process, unless if there are pipe.
* The command: export + unset + cd + exit
        is not execute if there are a pipe (before or after)
        or a redirection (>, ):
            in (>) case they create a empty file

* The command: echo + pwd + env
        is not execute if there are a pipe (after only)
        it execute if there are a redirection (>)
            in (>) case they create a file and put the output in it.

*/