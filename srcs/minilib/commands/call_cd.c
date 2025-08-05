/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   call_cd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dal-mahr <dal-mahr@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 08:53:57 by dal-mahr          #+#    #+#             */
/*   Updated: 2025/07/24 08:53:58 by dal-mahr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

void call_cd(t_minishell *mini, char **argv)
{
    char *path = argv[1];
    
    // Check if cd is being run in a pipe
    if (mini->pipex_count > 0)
    {
        ft_putstr_fd("minishell: cd: cannot change directory in pipe\n", STDERR_FILENO);
        mini->exit_status = 1;
        return;
    }
    
    // Handle multiple arguments
    if (argv[2])
    {
        ft_putstr_fd("minishell: cd: too many arguments\n", STDERR_FILENO);
        mini->exit_status = 1;
        return;
    }
    
    // If no argument, go to HOME
    if (!path)
        path = get_value_env(mini, "HOME");
    
    // Check if HOME is set when needed
    if (!path)
    {
        ft_putstr_fd("minishell: cd: HOME not set\n", STDERR_FILENO);
        mini->exit_status = 1;
        return;
    }
    
    // Handle special cases
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
    else if (ft_strcmp(path, "~") == 0)
    {
        path = get_value_env(mini, "HOME");
        if (!path)
        {
            ft_putstr_fd("minishell: cd: HOME not set\n", STDERR_FILENO);
            mini->exit_status = 1;
            return;
        }
    }
    
    // Try to change directory
    if (chdir(path) != 0)
    {
        ft_putstr_fd("minishell: cd: ", STDERR_FILENO);
        ft_putstr_fd(path, STDERR_FILENO);
        ft_putstr_fd(": ", STDERR_FILENO);
        if (access(path, F_OK) != 0)
            ft_putstr_fd("No such file or directory\n", STDERR_FILENO);
        else if (access(path, R_OK) != 0)
            ft_putstr_fd("Permission denied\n", STDERR_FILENO);
        else
            ft_putstr_fd("Not a directory\n", STDERR_FILENO);
        mini->exit_status = 1;
        return;
    }

    // Update environment variables
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
        mini->exit_status = 0; // cd succeeded, just can't get cwd
        return;
    }

    mini->exit_status = 0;
}
