/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_child.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rahaf <rahaf@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 08:52:38 by dal-mahr          #+#    #+#             */
/*   Updated: 2025/08/18 11:55:43 by rahaf            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

static void	print_command_error(t_minishell *ms, int i, int status)
{
	if (status == 126)
	{
		if (is_directory(ms->cmd[i].argv[0]))
		{
			write(2, ms->cmd[i].argv[0], ft_strlen(ms->cmd[i].argv[0]));
			write(2, ": Is a directory\n", 17);
		}
		else
		{
			write(2, ms->cmd[i].argv[0], ft_strlen(ms->cmd[i].argv[0]));
			write(2, ": Permission denied\n", 20);
		}
	}
	else if (status == 127)
	{
		write(2, ms->cmd[i].argv[0], ft_strlen(ms->cmd[i].argv[0]));
		write(2, ": command not found\n", 20);
	}
	else if (status == 128)
	{
		write(2, ms->cmd[i].argv[0], ft_strlen(ms->cmd[i].argv[0]));
		write(2, ": Not a directory\n", 18);
	}
	else
	{
		write(2, ms->cmd[i].argv[0], ft_strlen(ms->cmd[i].argv[0]));
		write(2, ": No such file or directory\n", 28);
	}
}

void	execute_child_command_pipe(t_minishell *ms, t_pipe_data *data)
{
	char	*path;
	int		status;

	setup_child_pipes(data->pipefds, data->i, data->n);
	close_all_pipes(data->pipefds, data->n);
	handle_file_redirection(&ms->cmd[data->i]);
	if (!ms->cmd[data->i].argv
		|| !ms->cmd[data->i].argv[0]
		|| ms->cmd[data->i].argv[0][0] == '\0')
	{
		cleanup_child_process(ms);
		exit(0);
	}
	status = resolve_cmd_path_with_status(ms->cmd[data->i].argv[0], ms, &path);
	if (status != 0)
	{
		print_command_error(ms, data->i, status);
		cleanup_child_process(ms);
		exit(status);
	}
	execve(path, ms->cmd[data->i].argv, data->envp);
	perror("execve");
	free(path);
	cleanup_child_process(ms);
	exit(EXIT_FAILURE);
}
