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
    if (!path)
        path = getenv("HOME");

    if (chdir(path) != 0)
    {
        perror("minishell: cd");
        mini->exit_status = 1;
        return;
    }

    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != NULL)
    {
        set_env_value(mini, "OLDPWD", get_value_env(mini, "PWD"));
        set_env_value(mini, "PWD", cwd);
    }

    mini->exit_status = 0;
}
