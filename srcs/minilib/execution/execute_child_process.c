/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_child_process.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rahaf <rahaf@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 08:55:21 by dal-mahr          #+#    #+#             */
/*   Updated: 2025/08/13 18:49:27 by rahaf            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

void	execute_child_process(t_minishell *mini, t_cmd *cmd, int prev_fd,
	int *pipe_fds, int is_last, char **envp)
{
	char	*path;
	int		status;

	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);

	save_original_fds(cmd);
	if (handle_redirections(cmd, prev_fd, pipe_fds, is_last) < 0)
	{
		cleanup_child_process(mini);
		exit(1);
	}

	// Handle heredoc without command - just exit successfully
	if (!cmd->argv || !cmd->argv[0])
	{
		cleanup_child_process(mini);
		exit(0);
	}

	if (is_builtin(cmd->argv[0]))
	{
		execute_builtin_cmd(mini, cmd);
		restore_original_fds(cmd);
		cleanup_child_process(mini);
		exit(0);
	}

	status = resolve_cmd_path_with_status(cmd->argv[0], mini, &path);
	if (status != 0)
	{
		if (status == 126)
		{
			if (is_directory(cmd->argv[0]))
			{
				write(2, cmd->argv[0], ft_strlen(cmd->argv[0]));
				write(2, ": Is a directory\n", 17);
			}
			else
			{
				write(2, cmd->argv[0], ft_strlen(cmd->argv[0]));
				write(2, ": Permission denied\n", 20);
			}
		}
		else
		{
			if (cmd->argv[0][0] == '/' || 
				(cmd->argv[0][0] == '.' && ft_strchr(cmd->argv[0], '/')))
			{
				write(2, "minishell: No such file or directory: ", 38);
				write(2, cmd->argv[0], ft_strlen(cmd->argv[0]));
				write(2, "\n", 1);
			}
			else
			{
				write(2, "minishell: command not found: ", 30);
				write(2, cmd->argv[0], ft_strlen(cmd->argv[0]));
				write(2, "\n", 1);
			}
		}
		cleanup_child_process(mini);
		exit(status);
	}

	execve(path, cmd->argv, envp);
	perror("execve");
	free(path);
	cleanup_child_process(mini);
	exit(EXIT_FAILURE);
}