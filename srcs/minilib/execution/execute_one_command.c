/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_one_command.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dal-mahr <dal-mahr@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 08:55:21 by dal-mahr          #+#    #+#             */
/*   Updated: 2025/08/12 17:30:00 by dal-mahr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

static int	handle_empty_command(t_minishell *mini, t_cmd *cmd)
{
	if (cmd->argv[0][0] == '\0')
	{
		write(2, "minishell: : command not found\n", 32);
		mini->exit_status = 127;
		return (1);
	}
	return (0);
}

static int	handle_parent_builtin(t_minishell *mini, t_cmd *cmd)
{
	char	*parent_builtins[5];

	parent_builtins[0] = "export";
	parent_builtins[1] = "unset";
	parent_builtins[2] = "cd";
	parent_builtins[3] = "exit";
	parent_builtins[4] = NULL;
	if (is_builtin(cmd->argv[0]) && is_str_in_set(cmd->argv[0], parent_builtins))
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

static void	handle_child_process(t_minishell *mini, t_cmd *cmd, char **envp)
{
	char	*path;

	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	if (handle_redirections(cmd, -1, NULL, 1) < 0)
		exit(1);
	if (is_builtin(cmd->argv[0]))
	{
		execute_builtin_cmd(mini, cmd);
		exit(0);
	}
	path = resolve_cmd_path(cmd->argv[0], mini);
	if (!path)
	{
		write(2, cmd->argv[0], ft_strlen(cmd->argv[0]));
		write(2, ": command not found\n", 20);
		exit(127);
	}
	execve(path, cmd->argv, envp);
	perror("execve");
	exit(126);
}

static void	handle_parent_process(t_minishell *mini, pid_t pid)
{
	int	status;

	waitpid(pid, &status, 0);
	set_child_running(0);
	if (WIFEXITED(status))
		mini->exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		mini->exit_status = 128 + WTERMSIG(status);
}

void	execute_one_command(t_minishell *mini, char **envp)
{
	t_cmd	*cmd;
	pid_t	pid;

	cmd = mini->cmd;
	if (!cmd || !cmd->argv[0])
		return ;
	if (handle_empty_command(mini, cmd))
		return ;
	if (cmd->in_type == HERE_DOC && handle_heredoc(mini, cmd) < 0)
		return ;
	if (handle_parent_builtin(mini, cmd))
		return ;
	set_child_running(1);
	pid = fork();
	if (pid < 0)
	{
		set_child_running(0);
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

int	is_one_command(t_minishell *mini)
{
	return (mini->pipex_count == 0);
}