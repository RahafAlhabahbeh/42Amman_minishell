/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_child_process.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dal-mahr <dal-mahr@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 23:00:00 by rahaf             #+#    #+#             */
/*   Updated: 2025/08/19 04:50:42 by dal-mahr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

static void	handle_initial_redirections(t_minishell *mini, t_cmd *cmd)
{
	if (handle_redirections(cmd, -1, NULL, 1) < 0)
		exit(1);
	if (!cmd->argv || !cmd->argv[0])
	{
		cleanup_child_process(mini);
		exit(0);
	}
	if (is_builtin(cmd->argv[0]))
	{
		execute_builtin_cmd(mini, cmd);
		cleanup_child_process(mini);
		exit(mini->exit_status);
	}
}

static void	handle_command_not_found(t_minishell *mini, t_cmd *cmd, int status)
{
	write(2, cmd->argv[0], ft_strlen(cmd->argv[0]));
	if (cmd->argv[0][0] == '/' || ft_strchr(cmd->argv[0], '/'))
		write(2, ": No such file or directory\n", 28);
	else
		write(2, ": command not found\n", 20);
	cleanup_child_process(mini);
	exit(status);
}

static void	handle_status_errors(t_minishell *mini, t_cmd *cmd, int status)
{
	if (status == 126)
	{
		write(2, cmd->argv[0], ft_strlen(cmd->argv[0]));
		if (is_directory(cmd->argv[0]))
			write(2, ": Is a directory\n", 17);
		else
			write(2, ": Permission denied\n", 20);
		cleanup_child_process(mini);
		exit(status);
	}
	else if (status == 128)
	{
		write(2, cmd->argv[0], ft_strlen(cmd->argv[0]));
		write(2, ": Not a directory\n", 18);
		cleanup_child_process(mini);
		exit(126);
	}
	else
		handle_command_not_found(mini, cmd, status);
}

void	handle_child_process(t_minishell *mini, t_cmd *cmd, char **envp)
{
	char	*path;
	int		status;

	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	handle_initial_redirections(mini, cmd);
	status = resolve_cmd_path_with_status(cmd->argv[0], mini, &path);
	if (status != 0)
		handle_status_errors(mini, cmd, status);
	execve(path, cmd->argv, envp);
	perror("execve");
	free(path);
	cleanup_child_process(mini);
	free_env_array_2(envp); // New Dana for Shell Level
	exit(126);
}
