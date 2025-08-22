/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   call_cd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rahaf <rahaf@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 08:53:57 by dal-mahr          #+#    #+#             */
/*   Updated: 2025/08/22 23:53:39 by rahaf            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

char	*get_user_home_dir(void)
{
	char	*home;

	home = getenv("HOME");
	if (home)
		return (ft_strdup(home));
	return (NULL);
}

char	*handle_tilde_expansion(t_minishell *mini, char *path)
{
	char	*home;
	char	*home_to_free;
	char	*expanded;

	home = get_value_env(mini, "HOME");
	home_to_free = NULL;
	if (!home)
	{
		home_to_free = get_user_home_dir();
		home = home_to_free;
		if (!home)
		{
			ft_putstr_fd("minishell: cd: HOME not set\n", STDERR_FILENO);
			return (NULL);
		}
	}
	expanded = ft_strjoin(home, path + 1);
	if (home_to_free)
		free(home_to_free);
	return (expanded);
}

char	*handle_cd_home(t_minishell *mini)
{
	char	*path;

	path = get_value_env(mini, "HOME");
	if (!path)
	{
		ft_putstr_fd("minishell: cd: HOME not set\n", STDERR_FILENO);
		mini->exit_status = 1;
		return (NULL);
	}
	return (path);
}

char	*handle_cd_oldpwd(t_minishell *mini)
{
	char	*path;
	char	*path_copy;

	path = get_value_env(mini, "OLDPWD");
	if (!path)
	{
		ft_putstr_fd("minishell: cd: OLDPWD not set\n", STDERR_FILENO);
		mini->exit_status = 1;
		return (NULL);
	}
	ft_putstr_fd(path, STDOUT_FILENO);
	ft_putchar_fd('\n', STDOUT_FILENO);
	path_copy = ft_strdup(path);
	return (path_copy);
}

char	*resolve_cd_path(t_minishell *mini, char **argv)
{
	char	*path;

	if (argv[1] && argv[2])
	{
		ft_putstr_fd("minishell: cd: too many arguments\n", STDERR_FILENO);
		mini->exit_status = 1;
		return (NULL);
	}
	path = argv[1];
	if (!path)
		return (handle_cd_home(mini));
	else if (ft_strcmp(path, "-p") == 0)
	{
		ft_putstr_fd("minishell: cd: -p: invalid option\n", STDERR_FILENO);
		ft_putstr_fd("cd: usage: cd [-L|[-P [-e]] [-@]] [dir]\n",
			STDERR_FILENO);
		mini->exit_status = 2;
		return (NULL);
	}
	else if (ft_strcmp(path, "-") == 0)
		return (handle_cd_oldpwd(mini));
	else if (path[0] == '~')
		return (handle_tilde_expansion(mini, path));
	return (path);
}
