/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   resolve_path.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rahaf <rahaf@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 00:00:00 by dal-mahr          #+#    #+#             */
/*   Updated: 2025/08/13 18:59:49 by rahaf            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

int	find_in_paths_with_status(char **paths, char *cmd, char **path)
{
	char	*full;
	int		res;
	int		i;

	i = 0;
	while (paths[i])
	{
		full = join_path(paths[i], cmd);
		if (full)
		{
			res = check_candidate(full, cmd, path, paths);
			if (res != -1)
				return (res);
		}
		i++;
	}
	return (127);
}

int	check_direct_cmd(char *cmd, char **path)
{
	int	status;

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
	return (-1);
}

int	resolve_cmd_path_with_status(char *cmd, t_minishell *mini, char **path)
{
	char	*path_env;
	char	**paths;
	int		status;

	*path = NULL;
	status = check_direct_cmd(cmd, path);
	if (status != -1)
		return (status);
	path_env = get_value_env(mini, "PATH");
	if (!path_env)
		return (127);
	paths = ft_split(path_env, ':');
	if (!paths)
		return (127);
	status = find_in_paths_with_status(paths, cmd, path);
	free_paths_array(paths);
	return (127);
}
