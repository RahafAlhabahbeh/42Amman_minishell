/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   call_cd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dal-mahr <dal-mahr@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 08:53:57 by dal-mahr          #+#    #+#             */
/*   Updated: 2025/08/08 08:39:09 by dal-mahr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

void call_cd(t_minishell *mini, char **argv)
{
    char *path = argv[1];
    int in_pipe = (mini->pipex_count > 0);

    if (argv[2])
    {
        ft_putstr_fd("minishell: cd: too many arguments\n", STDERR_FILENO);
        mini->exit_status = 1;
        return;
    }

    if (!path)
        path = get_value_env(mini, "HOME");

    if (!path)
    {
        ft_putstr_fd("minishell: cd: HOME not set\n", STDERR_FILENO);
        mini->exit_status = 1;
        return;
    }

    if (ft_strcmp(path, "-") == 0)
    {
        path = get_value_env(mini, "OLDPWD");
        if (!path)
        {
            ft_putstr_fd("minishell: cd: OLDPWD not set\n", STDERR_FILENO);
            mini->exit_status = 1;
            return;
        }
    }

    else if (path[0] == '~')
    {
        char *home = get_value_env(mini, "HOME");
        if (!home)
        {
            ft_putstr_fd("minishell: cd: HOME not set\n", STDERR_FILENO);
            mini->exit_status = 1;
            return;
        }

        char *expanded = ft_strjoin(home, path + 1); // path + 1 skips the '~'
        if (!expanded)
        {
            ft_putstr_fd("minishell: cd: memory allocation failed\n", STDERR_FILENO);
            mini->exit_status = 1;
            return;
        }
        path = expanded;
    }

    if (access(path, F_OK) != 0)
    {
        ft_putstr_fd("minishell: cd: ", STDERR_FILENO);
        ft_putstr_fd(path, STDERR_FILENO);
        ft_putstr_fd(": No such file or directory\n", STDERR_FILENO);
        mini->exit_status = 1;
        return;
    }
    if (access(path, R_OK) != 0)
    {
        ft_putstr_fd("minishell: cd: ", STDERR_FILENO);
        ft_putstr_fd(path, STDERR_FILENO);
        ft_putstr_fd(": Permission denied\n", STDERR_FILENO);
        mini->exit_status = 1;
        return;
    }

    if (in_pipe)
    {
        mini->exit_status = 0;
        return;
    }

    if (chdir(path) != 0)
    {
        ft_putstr_fd("minishell: cd: ", STDERR_FILENO);
        ft_putstr_fd(path, STDERR_FILENO);
        ft_putstr_fd(": Not a directory\n", STDERR_FILENO);
        mini->exit_status = 1;
        return;
    }

    char *old_pwd = get_value_env(mini, "PWD");
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != NULL)
    {
        if (old_pwd)
            set_env_value(mini, "OLDPWD", old_pwd);
        set_env_value(mini, "PWD", cwd);
    }
    else
    {
        ft_putstr_fd("minishell: error retrieving current directory: ", STDERR_FILENO);
        ft_putstr_fd("getcwd: cannot access parent directories: No such file or directory\n", STDERR_FILENO);
        mini->exit_status = 0;
        return;
    }

    mini->exit_status = 0;
}
