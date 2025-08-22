/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_child.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rahaf <rahaf@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 08:52:38 by dal-mahr          #+#    #+#             */
/*   Updated: 2025/08/22 23:53:39 by rahaf            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

static void	print_status_126_error(t_minishell *ms, int i)
{
	if (is_directory(ms->cmd[i].argv[0]))
	{
		ft_putstr_fd(ms->cmd[i].argv[0], 2);
		ft_putstr_fd(": Is a directory\n", 2);
	}
	else
	{
		ft_putstr_fd(ms->cmd[i].argv[0], 2);
		ft_putstr_fd(": Permission denied\n", 2);
	}
}

static void	print_command_error(t_minishell *ms, int i, int status)
{
	if (status == 126)
		print_status_126_error(ms, i);
	else if (status == 127)
	{
		ft_putstr_fd(ms->cmd[i].argv[0], 2);
		ft_putstr_fd(": command not found\n", 2);
	}
	else if (status == 128)
	{
		ft_putstr_fd(ms->cmd[i].argv[0], 2);
		ft_putstr_fd(": Not a directory\n", 2);
	}
	else
	{
		ft_putstr_fd(ms->cmd[i].argv[0], 2);
		ft_putstr_fd(": No such file or directory\n", 2);
	}
}

static void	handle_empty_cmd_pipe(t_minishell *ms, int cmd_index)
{
	handle_file_redirection(&ms->cmd[cmd_index]);
	cleanup_child_process(ms);
	exit(0);
}

static void	execute_cmd_with_path(t_minishell *ms, t_pipe_data *data,
	char *path)
{
	execve(path, ms->cmd[data->i].argv, data->envp);
	ft_putstr_fd("minishell: execve error\n", 2);
	free(path);
	cleanup_child_process(ms);
	exit(1);
}

void	execute_child_command_pipe(t_minishell *ms, t_pipe_data *data)
{
	char	*path;
	int		status;

	setup_child_pipes(data->pipefds, data->i, data->n);
	close_all_pipes(data->pipefds, data->n);
	if (!ms->cmd[data->i].argv || !ms->cmd[data->i].argv[0]
		|| ms->cmd[data->i].argv[0][0] == '\0')
		handle_empty_cmd_pipe(ms, data->i);
	handle_file_redirection(&ms->cmd[data->i]);
	status = resolve_cmd_path_with_status(ms->cmd[data->i].argv[0], ms, &path);
	if (status != 0)
	{
		print_command_error(ms, data->i, status);
		cleanup_child_process(ms);
		exit(status);
	}
	execute_cmd_with_path(ms, data, path);
}
