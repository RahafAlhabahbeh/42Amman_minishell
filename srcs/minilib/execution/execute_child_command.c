/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_child_command.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rahaf <rahaf@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 00:00:00 by dal-mahr          #+#    #+#             */
/*   Updated: 2025/08/15 17:53:42 by rahaf            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

void	close_pipe_fds(int *pipe_fds)
{
	if (pipe_fds[0] != -1)
	{
		close(pipe_fds[0]);
		pipe_fds[0] = -1;
	}
	if (pipe_fds[1] != -1)
	{
		close(pipe_fds[1]);
		pipe_fds[1] = -1;
	}
}

void	process_heredocs(t_minishell *mini)
{
	t_cmd	*cmd;
	int		i;

	i = 0;
	while (i <= mini->pipex_count)
	{
		cmd = &mini->cmd[i];
		if (cmd->in_type == HERE_DOC && handle_heredoc(mini, cmd) < 0)
			return ;
		i++;
	}
}

static void	handle_child_builtins(t_minishell *mini, t_cmd *cmd, int i)
{
	char	*child_builtins[2];

	if (!cmd->argv || !cmd->argv[0])
	{
		cleanup_child_process(mini);
		exit(0);
	}
	if (is_builtin(cmd->argv[0]))
	{
		child_builtins[0] = "unset";
		child_builtins[1] = NULL;
		if (is_str_in_set(cmd->argv[0], child_builtins)
			&& mini->pipex_count > 0)
		{
			cleanup_child_process(mini);
			exit(0);
		}
		execute_builtin(mini, i);
		cleanup_child_process(mini);
		exit(mini->exit_status);
	}
}

static void	handle_command_errors(t_minishell *mini, t_cmd *cmd, int status)
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
		write(2, cmd->argv[0], ft_strlen(cmd->argv[0]));
		if (cmd->argv[0][0] == '/' || (cmd->argv[0][0] == '.' &&
			ft_strchr(cmd->argv[0], '/')))
			write(2, ": No such file or directory\n", 28);
		else
			write(2, ": command not found\n", 20);
	}
	cleanup_child_process(mini);
	exit(status);
}

void	execute_child_command(t_minishell *mini, t_cmd *cmd, int i, char **envp)
{
	char	*path;
	int		status;

	handle_child_builtins(mini, cmd, i);
	status = resolve_cmd_path_with_status(cmd->argv[0], mini, &path);
	if (status != 0)
		handle_command_errors(mini, cmd, status);
	execve(path, cmd->argv, envp);
	perror("execve");
	free(path);
	cleanup_child_process(mini);
	exit(126);
}
/*
void	execute_child_command(t_minishell *mini, t_cmd *cmd, int i, char **envp)
{
	char	*path;
	int		status;
	char	*child_builtins[3];

	if (!cmd->argv || !cmd->argv[0])
	{
		cleanup_child_process(mini);
		exit(0);
	}
	if (is_builtin(cmd->argv[0]))
	{
		child_builtins[0] = "unset";
		child_builtins[1] = NULL;
		if (is_str_in_set(cmd->argv[0], child_builtins)
			&& mini->pipex_count > 0)
		{
			cleanup_child_process(mini);
			exit(0);
		}
		execute_builtin(mini, i);
		cleanup_child_process(mini);
		exit(mini->exit_status);
	}
	else
	{
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
				write(2, cmd->argv[0], ft_strlen(cmd->argv[0]));
				if (cmd->argv[0][0] == '/' ||
					(cmd->argv[0][0] == '.' && ft_strchr(cmd->argv[0], '/')))
					write(2, ": No such file or directory\n", 28);
				else
					write(2, ": command not found\n", 20);
			}
			cleanup_child_process(mini);
			exit(status);
		}
		execve(path, cmd->argv, envp);
		perror("execve");
		free(path);
		cleanup_child_process(mini);
		exit(126);
	}
	cleanup_child_process(mini);
	exit(0);
}
*/
