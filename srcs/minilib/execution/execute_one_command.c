/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_one_command.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rahaf <rahaf@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 08:55:21 by dal-mahr          #+#    #+#             */
/*   Updated: 2025/08/20 14:10:43 by rahaf            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

int handle_empty_command2(t_minishell *mini, t_cmd *cmd)
{
	if (!cmd->argv || !cmd->argv[0] || cmd->argv[0][0] == '\0')
	{
		if (cmd->input_file_name || cmd->output_file_name)
		{
			// Process redirections to check for errors
			if (handle_redirections(cmd, -1, NULL, 1) < 0)
			{
				mini->exit_status = 1;
			}
			else
			{
				mini->exit_status = 0;
			}
			return (1);
		}
		ft_putstr_fd("minishell: ", 2);
		if (cmd->argv && cmd->argv[0])
			ft_putstr_fd(cmd->argv[0], 2);
		else
			ft_putstr_fd("command", 2);
		ft_putstr_fd(": command not found\n", 2);
		mini->exit_status = 127;
		return (1);
	}
	return (0);
}

int handle_parent_builtin_child(t_minishell *mini, t_cmd *cmd)
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

void handle_parent_process(t_minishell *mini, pid_t pid)
{
	int status;
	int wait_result;
	struct sigaction sa;

	ft_memset(&sa, 0, sizeof(sa));
	sa.sa_handler = SIG_IGN;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, NULL);
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
			if (WTERMSIG(status) == SIGINT)
				write(1, "\n", 1);
			g_received_signal = 0;
		}
	}
}

void execute_one_command(t_minishell *mini, char **envp)
{
	t_cmd *cmd;
	pid_t pid;

	(void) envp;
	cmd = mini->cmd;
	
	
	if (!cmd)
		return;
	if (cmd->in_type == HERE_DOC && handle_heredoc(mini, cmd) < 0)
		return;
	if (handle_empty_command2(mini, cmd))
		return;
	if (handle_parent_builtin_child(mini, cmd))
		return;
	set_child_running(1);
	pid = fork();
	if (pid < 0)
	{
		perror("fork");
		mini->exit_status = 1;
		return;
	}
	// else if (pid == 0)
	// 	handle_child_process(mini, cmd, envp);
	else if (pid == 0) // New 
	{
		char **child_env;

		child_env = convert_env_to_array(mini->env_list); // new helper
		mini->child_env = child_env;
		handle_child_process(mini, cmd, child_env);
		free_env_array_2(child_env); // free after use if execve fails
	}
	else
		handle_parent_process(mini, pid);
	cleanup_heredoc_files(mini);
}
