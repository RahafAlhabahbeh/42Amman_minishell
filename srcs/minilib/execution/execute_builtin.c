/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_builtin.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dal-mahr <dal-mahr@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 08:55:21 by dal-mahr          #+#    #+#             */
/*   Updated: 2025/08/04 09:47:41 by dal-mahr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

int is_builtin(char *cmd)
{
    if (!cmd)
        return 0;
    return (!ft_strcmp(cmd, "echo") || !ft_strcmp(cmd, "cd") || !ft_strcmp(cmd, "pwd") ||
            !ft_strcmp(cmd, "env") || !ft_strcmp(cmd, "export") || !ft_strcmp(cmd, "unset") ||
            !ft_strcmp(cmd, "exit"));
}


void execute_builtin_cmd(t_minishell *mini, t_cmd *cmd)
{
    if (!cmd || !cmd->argv || !cmd->argv[0])
        return;

    char *name = cmd->argv[0];

    if (!ft_strcmp(name, "echo"))
        call_echo(mini, cmd->argv);
    else if (!ft_strcmp(name, "pwd"))
        call_pwd(mini);
    else if (!ft_strcmp(name, "env"))
        call_env(mini, cmd->argv);
    else if (!ft_strcmp(name, "export"))
        call_export(mini, cmd->argv);
    else if (!ft_strcmp(name, "unset"))
        call_unset(mini, cmd->argv);
    else if (!ft_strcmp(name, "exit"))
        call_exit(mini, cmd->argv);
    else if (!ft_strcmp(name, "cd"))
        call_cd(mini, cmd->argv);
    // else if (!ft_strcmp(name, "cd"))
    // {
    //     // cd in child process should show error
    //     ft_putstr_fd("minishell: cd: cannot change directory in pipe\n", STDERR_FILENO);
    //     mini->exit_status = 1;
    // }
}

void execute_builtin(t_minishell *minishell, int i)
{
    if (i < 0 || i > minishell->pipex_count || !minishell->cmd[i].argv || !minishell->cmd[i].argv[0])
        return;

    char *name = minishell->cmd[i].argv[0];

    if (!ft_strcmp(name, "echo"))
        call_echo(minishell, minishell->cmd[i].argv);
    else if (!ft_strcmp(name, "pwd"))
        call_pwd(minishell);
    else if (!ft_strcmp(name, "env"))
        call_env(minishell, minishell->cmd[i].argv);
    else if (!ft_strcmp(name, "export"))
        call_export(minishell, minishell->cmd[i].argv);
    else if (!ft_strcmp(name, "unset"))
        call_unset(minishell, minishell->cmd[i].argv);
    else if (!ft_strcmp(name, "exit"))
        call_exit(minishell, minishell->cmd[i].argv);
    else if (!ft_strcmp(name, "cd"))
        call_cd(minishell, minishell->cmd[i].argv);

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