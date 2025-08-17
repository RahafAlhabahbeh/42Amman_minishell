/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   resolve_path_utils2.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rahaf <rahaf@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 00:00:00 by dal-mahr          #+#    #+#             */
/*   Updated: 2025/08/17 17:14:47 by rahaf            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

char	*check_path(char *full, char *cmd, char **paths)
{
	int	status;

	(void) cmd;
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
	return (NULL);
}

char	*find_in_paths(char **paths, char *cmd)
{
	char	*full;
	char	*res;
	int		i;

	i = 0;
	while (paths[i])
	{
		full = join_path(paths[i], cmd);
		if (full)
		{
			res = check_path(full, cmd, paths);
			if (res)
				return (res);
		}
		i++;
	}
	return (NULL);
}

char	*resolve_cmd_path(char *cmd, t_minishell *mini)
{
	char	*path_env;
	char	**paths;
	char	*full;

	if (!cmd)
		return (NULL);
	full = check_absolute_path(cmd);
	if (full || cmd[0] == '/' || (cmd[0] == '.' && cmd[1]))
		return (full);
	path_env = get_value_env(mini, "PATH");
	if (!path_env)
		return (NULL);
	paths = ft_split(path_env, ':');
	if (!paths)
		return (NULL);
	full = find_in_paths(paths, cmd);
	free_paths_array(paths);
	return (full);
}

int	check_candidate(char *full, char *cmd, char **path, char **paths)
{
	int	status;

	(void) cmd;
	(void) paths;
	if (is_directory(full))
	{
		free(full);
		return (-1);
	}
	status = is_executable(full);
	if (status == 0)
	{
		*path = full;
		return (0);
	}
	else if (status == -2)
	{
		free(full);
		return (126);
	}
	free(full);
	return (-1);
}
