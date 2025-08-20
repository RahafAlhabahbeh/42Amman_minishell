/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_command.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rahaf <rahaf@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 00:00:00 by dal-mahr          #+#    #+#             */
/*   Updated: 2025/08/20 14:24:06 by rahaf            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

void	handle_child_process2(t_minishell *mini,
	t_exec_vars *vars, char **child_env)
{
	set_in_child_process(1);
	mini->child_env = child_env;
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	if (handle_redirections(vars->cmd, vars->prev_fd, vars->pipefd,
			vars->i == mini->pipex_count) < 0)
	{
		free_env_array_2(child_env);
		cleanup_child_process(mini);
		exit(1);
	}
	if (!vars->cmd->argv || !vars->cmd->argv[0])
	{
		free_env_array_2(child_env);
		cleanup_child_process(mini);
		exit(0);
	}
	if (!vars->cmd->argv[0][0] || (vars->cmd->in_type == REDIR_IN
		&& !vars->cmd->argv[0][0]))
	{
		handle_empty_command(mini, vars->cmd, child_env);
	}
	execute_child_command(mini, vars->cmd, vars->i, child_env);
}

int	handle_parent_builtin(t_minishell *mini, t_exec_vars *vars, pid_t *pids)
{
	if (vars->cmd->argv && vars->cmd->argv[0] && is_builtin(vars->cmd->argv[0])
		&& should_run_builtin_in_parent(vars->cmd, vars->i, mini->pipex_count))
	{
		save_original_fds(vars->cmd);
		if (handle_redirections(vars->cmd, vars->prev_fd, vars->pipefd,
				vars->i == mini->pipex_count) == 0)
			execute_builtin(mini, vars->i);
		else if (vars->i < mini->pipex_count)
			close_pipe_fds(vars->pipefd);
		restore_original_fds(vars->cmd);
		pids[vars->i] = -2;
		return (1);
	}
	return (0);
}

void	execute_command(t_minishell *mini, char **envp)
{
	if (mini->pipex_count == 0)
		execute_one_command(mini, envp);
	else
		multiple_command_execution(mini, envp);
	setup_signals();
}
/*
void	handle_child_process2(t_minishell *mini, t_cmd *cmd, int prev_fd,
	int *pipefd, int i, char **envp)
{
	set_in_child_process(1);
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	if (handle_redirections(cmd, prev_fd, pipefd,
			i == mini->pipex_count) < 0)
		exit(1);
	if (!cmd->argv || !cmd->argv[0])
	{
		cleanup_child_process(mini);
		exit(0);
	}
	handle_empty_command(cmd);
	execute_child_command(mini, cmd, i, envp);
}

int	handle_parent_builtin(t_minishell *mini, t_cmd *cmd, int prev_fd,
	int *pipefd, int i, pid_t *pids)
{
	if (cmd->argv && cmd->argv[0] && is_builtin(cmd->argv[0])
		&& should_run_builtin_in_parent(cmd, i, mini->pipex_count))
	{
		save_original_fds(cmd);
		if (handle_redirections(cmd, prev_fd, pipefd,
				i == mini->pipex_count) == 0)
			execute_builtin(mini, i);
		else
		{
			if (i < mini->pipex_count)
				close_pipe_fds(pipefd);
		}
		restore_original_fds(cmd);
		pids[i] = -2;
		return (1);
	}
	return (0);
}

void	execute_command(t_minishell *mini, char **envp)
{
	if (mini->pipex_count == 0)
		execute_one_command(mini, envp);
	else
		multiple_command_execution(mini, envp);
	setup_signals();
}
*/
