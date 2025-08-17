/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   resolve_path_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rahaf <rahaf@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 00:00:00 by dal-mahr          #+#    #+#             */
/*   Updated: 2025/08/13 18:59:49 by rahaf            ###   ########.fr       */
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

int	is_executable(const char *path)
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

static void	print_error(char *cmd, char *msg, int len)
{
	write(2, cmd, ft_strlen(cmd));
	write(2, msg, len);
}

char	*check_absolute_path(char *cmd)
{
	int	status;

	if (!(cmd[0] == '/' || (cmd[0] == '.' && ft_strchr(cmd, '/'))))
		return (NULL);
	if (is_directory(cmd))
	{
		print_error(cmd, ": Is a directory\n", 17);
		return (NULL);
	}
	status = is_executable(cmd);
	if (status == 0)
		return (ft_strdup(cmd));
	if (status == -1)
		print_error(cmd, ": No such file or directory\n", 28);
	else if (status == -2)
		print_error(cmd, ": Permission denied\n", 20);
	return (NULL);
}

void	free_paths_array(char **paths)
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
