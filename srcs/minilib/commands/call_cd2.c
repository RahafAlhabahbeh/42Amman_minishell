/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   call_cd2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rahaf <rahaf@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 15:53:31 by rahaf             #+#    #+#             */
/*   Updated: 2025/08/15 17:53:42 by rahaf            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

static int	check_path_access(t_minishell *mini, char *path, char **argv)
{
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
	return (0);
}

static void	update_pwd_env(t_minishell *mini)
{
	char	*old_pwd;
	char	cwd[1024];

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
}

static int	validate_and_change_directory(t_minishell *mini, char *path,
	char **argv)
{
	if (check_path_access(mini, path, argv))
		return (1);
	if (mini->pipex_count > 0)
	{
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
	update_pwd_env(mini);
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
