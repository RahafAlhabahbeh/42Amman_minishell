/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   resolve_path.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dal-mahr <dal-mahr@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 00:00:00 by dal-mahr          #+#    #+#             */
/*   Updated: 2025/08/12 17:30:00 by dal-mahr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

int	is_directory(const char *path)
{
	struct stat	st;

	if (stat(path, &st) == 0)
		return (S_ISDIR(st.st_mode));
	return (0);
}

static int	is_executable(const char *path)
{
	struct stat	sb;

	if (access(path, F_OK) != 0)
		return (-1);
	if (access(path, X_OK) != 0)
		return (-2);
	if (stat(path, &sb) == 0 && S_ISREG(sb.st_mode))
		return (0);
	return (-2);
}

static char	*check_absolute_path(char *cmd)
{
	int	status;

	if (cmd[0] == '/' || (cmd[0] == '.' && ft_strchr(cmd, '/')))
	{
		if (is_directory(cmd))
		{
			write(2, cmd, ft_strlen(cmd));
			write(2, ": Is a directory\n", 17);
			return (NULL);
		}
		status = is_executable(cmd);
		if (status == 0)
			return (ft_strdup(cmd));
		if (status == -1)
		{
			write(2, cmd, ft_strlen(cmd));
			write(2, ": No such file or directory\n", 28);
			return (NULL);
		}
		if (status == -2)
		{
			write(2, cmd, ft_strlen(cmd));
			write(2, ": Permission denied\n", 20);
			return (NULL);
		}
		return (NULL);
	}
	return (NULL);
}

static void	free_paths_array(char **paths)
{
	int	i;

	i = 0;
	while (paths[i])
	{
		free(paths[i]);
		i++;
	}
	free(paths);
}

char	*resolve_cmd_path(char *cmd, t_minishell *mini)
{
	char	*path_env;
	char	**paths;
	char	*full;
	int		status;
	int		i;

	if (!cmd)
		return (NULL);
	full = check_absolute_path(cmd);
	if (full || cmd[0] == '/' || (cmd[0] == '.' && cmd[1]))
		return (full);
	path_env = get_value_env(mini, "PATH");
	if (!path_env)
		return (NULL);
	paths = ft_split(path_env, ':');
	i = 0;
	while (paths[i])
	{
		full = join_path(paths[i], cmd);
		if (!full)
		{
			i++;
			continue ;
		}
		status = is_executable(full);
		if (status == 0)
		{
			free_paths_array(paths);
			return (full);
		}
		else if (status == -2)
		{
			write(2, cmd, ft_strlen(cmd));
			write(2, ": Permission denied\n", 20);
			free_paths_array(paths);
			free(full);
			return (NULL);
		}
		free(full);
		i++;
	}
	free_paths_array(paths);
	return (NULL);
}

int	resolve_cmd_path_with_status(char *cmd, t_minishell *mini, char **path)
{
	char	*path_env;
	char	**paths;
	char	*full;
	int		status;
	int		i;

	*path = NULL;
	if (!cmd)
		return (127);
	if (cmd[0] == '/' || (cmd[0] == '.' && ft_strchr(cmd, '/')))
	{
		status = is_executable(cmd);
		if (status == 0)
		{
			*path = ft_strdup(cmd);
			return (0);
		}
		if (status == -1)
			return (127);
		return (126);
	}
	path_env = get_value_env(mini, "PATH");
	if (!path_env)
		return (127);
	paths = ft_split(path_env, ':');
	i = 0;
	while (paths[i])
	{
		full = join_path(paths[i], cmd);
		if (!full)
		{
			i++;
			continue ;
		}
		if (is_directory(full))
		{
			free(full);
			i++;
			continue ;
		}
		status = is_executable(full);
		if (status == 0)
		{
			free_paths_array(paths);
			*path = full;
			return (0);
		}
		else if (status == -2)
		{
			free_paths_array(paths);
			free(full);
			return (126);
		}
		free(full);
		i++;
	}
	free_paths_array(paths);
	return (127);
}
