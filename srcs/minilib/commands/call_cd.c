/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   call_cd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dal-mahr <dal-mahr@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 08:53:57 by dal-mahr          #+#    #+#             */
/*   Updated: 2025/08/12 17:30:00 by dal-mahr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"
#include <pwd.h>
#include <unistd.h>

static char	*get_user_home_dir(void)
{
	struct passwd	*pw;

	pw = getpwuid(getuid());
	if (pw && pw->pw_dir)
		return (ft_strdup(pw->pw_dir));
	return (NULL);
}

static char	*handle_tilde_expansion(t_minishell *mini, char *path)
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

static char	*resolve_cd_path(t_minishell *mini, char **argv)
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
	{
		path = get_value_env(mini, "HOME");
		if (!path)
		{
			ft_putstr_fd("minishell: cd: HOME not set\n", STDERR_FILENO);
			mini->exit_status = 1;
			return (NULL);
		}
	}
	else if (ft_strcmp(path, "-") == 0)
	{
		path = get_value_env(mini, "OLDPWD");
		if (!path)
		{
			ft_putstr_fd("minishell: cd: OLDPWD not set\n", STDERR_FILENO);
			mini->exit_status = 1;
			return (NULL);
		}
	}
	else if (path[0] == '~')
		return (handle_tilde_expansion(mini, path));
	return (path);
}

static int	validate_and_change_directory(t_minishell *mini, char *path,
	char **argv)
{
	char	*old_pwd;
	char	cwd[1024];

	if (access(path, F_OK) != 0)
	{
		ft_putstr_fd("minishell: cd: ", STDERR_FILENO);
		ft_putstr_fd(path, STDERR_FILENO);
		ft_putstr_fd(": No such file or directory\n", STDERR_FILENO);
		mini->exit_status = 1;
		if (path != argv[1] && path[0] == '~')
			free(path);
		return (1);
	}
	if (access(path, R_OK) != 0)
	{
		ft_putstr_fd("minishell: cd: ", STDERR_FILENO);
		ft_putstr_fd(path, STDERR_FILENO);
		ft_putstr_fd(": Permission denied\n", STDERR_FILENO);
		mini->exit_status = 1;
		if (path != argv[1] && path[0] == '~')
			free(path);
		return (1);
	}
	if (mini->pipex_count > 0)
	{
		mini->exit_status = 0;
		if (path != argv[1] && path[0] == '~')
			free(path);
		return (1);
	}
	if (chdir(path) != 0)
	{
		ft_putstr_fd("minishell: cd: ", STDERR_FILENO);
		ft_putstr_fd(path, STDERR_FILENO);
		ft_putstr_fd(": Not a directory\n", STDERR_FILENO);
		mini->exit_status = 1;
		return (1);
	}
	old_pwd = get_value_env(mini, "PWD");
	if (getcwd(cwd, sizeof(cwd)) != NULL)
	{
		if (old_pwd)
			set_env_value(mini, "OLDPWD", old_pwd);
		set_env_value(mini, "PWD", cwd);
	}
	else
	{
		ft_putstr_fd("minishell: error retrieving current directory: ",
			STDERR_FILENO);
		ft_putstr_fd("getcwd: cannot access parent directories: ",
			STDERR_FILENO);
		ft_putstr_fd("No such file or directory\n", STDERR_FILENO);
	}
	mini->exit_status = 0;
	return (0);
}

void	call_cd(t_minishell *mini, char **argv)
{
	char	*path;

	path = resolve_cd_path(mini, argv);
	if (!path)
		return ;
	validate_and_change_directory(mini, path, argv);
	if (path != argv[1] && path[0] == '~')
		free(path);
}