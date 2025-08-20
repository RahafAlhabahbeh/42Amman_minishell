/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_child_process.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rahaf <rahaf@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 23:00:00 by rahaf             #+#    #+#             */
/*   Updated: 2025/08/20 09:21:03 by rahaf            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

static void	handle_initial_redirections(t_minishell *mini, t_cmd *cmd, char **envp)
{
	if (handle_redirections(cmd, -1, NULL, 1) < 0)
	{
		free_env_array_2(envp);
		cleanup_child_process(mini);
		exit(1);
	}
	if (!cmd->argv || !cmd->argv[0])
	{
		free_env_array_2(envp);
		cleanup_child_process(mini);
		exit(0);
	}
	if (is_builtin(cmd->argv[0]))
	{
		execute_builtin_cmd(mini, cmd);
		free_env_array_2(envp);
		cleanup_child_process(mini);
		exit(mini->exit_status);
	}
}

static void	handle_command_not_found(t_minishell *mini, t_cmd *cmd, int status)
{
	ft_putstr_fd(cmd->argv[0], 2);
	if (cmd->argv[0][0] == '/' || ft_strchr(cmd->argv[0], '/'))
		ft_putstr_fd(": No such file or directory\n", 2);
	else
		ft_putstr_fd(": command not found\n", 2);
	cleanup_child_process(mini);
	exit(status);
}

static void	handle_status_errors(t_minishell *mini, t_cmd *cmd, int status)
{
	if (status == 126)
	{
		ft_putstr_fd(cmd->argv[0], 2);
		if (is_directory(cmd->argv[0]))
			ft_putstr_fd(": Is a directory\n", 2);
		else
			ft_putstr_fd(": Permission denied\n", 2);
		cleanup_child_process(mini);
		exit(status);
	}
	else if (status == 128)
	{
		ft_putstr_fd(cmd->argv[0], 2);
		ft_putstr_fd(": Not a directory\n", 2);
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
	handle_initial_redirections(mini, cmd, envp);
	status = resolve_cmd_path_with_status(cmd->argv[0], mini, &path);
	if (status != 0)
	{
		free_env_array_2(envp);
		handle_status_errors(mini, cmd, status);
	}
	execve(path, cmd->argv, envp);
	ft_putstr_fd("minishell: execve error\n", 2);
	free(path);
	cleanup_child_process(mini);
	free_env_array_2(envp);
	exit(126);
}
