/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_child_process.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dal-mahr <dal-mahr@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 08:55:21 by dal-mahr          #+#    #+#             */
/*   Updated: 2025/08/12 17:30:00 by dal-mahr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

void	execute_child_process(t_minishell *mini, t_cmd *cmd, int prev_fd,
	int *pipe_fds, int is_last, char **envp)
{
	char	*path;

	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);

	save_original_fds(cmd);
	handle_redirections(cmd, prev_fd, pipe_fds, is_last);

	if (is_builtin(cmd->argv[0]))
	{
		execute_builtin_cmd(mini, cmd);
		restore_original_fds(cmd);
		exit(0);
	}

	path = resolve_cmd_path(cmd->argv[0], mini);
	if (!path)
	{
		write(2, "minishell: command not found: ", 30);
		write(2, cmd->argv[0], ft_strlen(cmd->argv[0]));
		write(2, "\n", 1);
		exit(127);
	}

	execve(path, cmd->argv, envp);
	perror("execve");
	exit(EXIT_FAILURE);
}