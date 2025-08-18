/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   resolve_path.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rahaf <rahaf@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 00:00:00 by dal-mahr          #+#    #+#             */
/*   Updated: 2025/08/18 11:55:43 by rahaf            ###   ########.fr       */
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
	int			status;
	struct stat	st;
	char		*without_slash;
	int			len;

	if (!cmd)
		return (127);
	if (cmd[0] == '/' || ft_strchr(cmd, '/'))
	{
		len = ft_strlen(cmd);
		// Check if it ends with slash and if so, check if base path exists as file
		if (len > 1 && cmd[len - 1] == '/')
		{
			without_slash = ft_substr(cmd, 0, len - 1);
			if (without_slash && access(without_slash, F_OK) == 0)
			{
				if (stat(without_slash, &st) == 0 && !S_ISDIR(st.st_mode))
				{
					free(without_slash);
					return (128); // Not a directory
				}
			}
			free(without_slash);
		}
		
		// Check if path exists first
		if (access(cmd, F_OK) != 0)
			return (127); // No such file or directory
		
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
	return (status);
}
