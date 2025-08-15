/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_one_command.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rahaf <rahaf@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 08:55:21 by dal-mahr          #+#    #+#             */
/*   Updated: 2025/08/15 17:53:42 by rahaf            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

int	handle_empty_command2(t_minishell *mini, t_cmd *cmd)
{
	if (!cmd->argv || !cmd->argv[0] || cmd->argv[0][0] == '\0')
	{
		write(2, cmd->argv[0], ft_strlen(cmd->argv[0]));
		write(2, ": command not found\n", 20);
		mini->exit_status = 127;
		return (1);
	}
	return (0);
}

int	handle_parent_builtin_child(t_minishell *mini, t_cmd *cmd)
{
	if (is_builtin(cmd->argv[0]))
	{
		save_original_fds(cmd);
		if (handle_redirections(cmd, -1, NULL, 1) < 0)
		{
			restore_original_fds(cmd);
			mini->exit_status = 1;
			return (1);
		}
		execute_builtin_cmd(mini, cmd);
		restore_original_fds(cmd);
		return (1);
	}
	return (0);
}

void	handle_parent_process(t_minishell *mini, pid_t pid)
{
	int	status;
	int	wait_result;

	status = 0;
	wait_result = waitpid(pid, &status, 0);
	set_child_running(0);
	if (wait_result > 0)
	{
		if (WIFEXITED(status))
			mini->exit_status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
		{
			mini->exit_status = 128 + WTERMSIG(status);
			printf("\n");
			g_received_signal = 0;
		}
	}
}

void	execute_one_command(t_minishell *mini, char **envp)
{
	t_cmd	*cmd;
	pid_t	pid;

	cmd = mini->cmd;
	if (!cmd || (cmd->in_type == HERE_DOC && handle_heredoc(mini, cmd) < 0)
		|| (!cmd->argv || !cmd->argv[0]) || handle_empty_command2(mini, cmd)
		|| handle_parent_builtin_child(mini, cmd))
	{
		if (!cmd->argv || !cmd->argv[0])
			mini->exit_status = 0;
		return ;
	}
	set_child_running(1);
	pid = fork();
	if (pid < 0)
	{
		perror("fork");
		mini->exit_status = 1;
		return ;
	}
	else if (pid == 0)
		handle_child_process(mini, cmd, envp);
	else
		handle_parent_process(mini, pid);
	cleanup_heredoc_files(mini);
}
