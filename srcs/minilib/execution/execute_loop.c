/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_loop.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rahaf <rahaf@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 00:00:00 by dal-mahr          #+#    #+#             */
/*   Updated: 2025/08/21 16:05:06 by rahaf            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

static int	handle_empty_cmd_redirections(t_minishell *mini, t_exec_vars *vars)
{
	int	redir_result;

	if ((!vars->cmd->argv || !vars->cmd->argv[0]
			|| vars->cmd->argv[0][0] == '\0')
		&& (vars->cmd->input_file_name || vars->cmd->output_file_name
			|| vars->cmd->redirections))
	{
		if (mini->pipex_count > 0 && vars->cmd->in_type == REDIR_IN
			&& vars->cmd->input_file_name && vars->i < mini->pipex_count)
			return (0);
		redir_result = handle_empty_redirections(vars->cmd);
		if (redir_result < 0)
			mini->exit_status = 1;
		else
			mini->exit_status = 0;
		return (1);
	}
	return (0);
}

static int	prepare_pipe_and_sigint(t_minishell *mini, t_exec_vars *vars)
{
	int	empty_result;

	if (check_sigint_received())
	{
		mini->exit_status = 130;
		return (1);
	}
	vars->cmd = &mini->cmd[vars->i];
	empty_result = handle_empty_cmd_redirections(mini, vars);
	if (empty_result != 0)
		return (empty_result);
	if (create_pipe_if_needed(mini, vars))
		return (1);
	setup_signal_handling();
	return (0);
}

static int	handle_fork_and_execute(t_minishell *mini, t_exec_vars *vars,
	pid_t *pids)
{
	char	**child_env;

	if (handle_parent_builtin(mini, vars, pids))
		return (execute_parent_process(vars, vars->i == mini->pipex_count));
	vars->pid = fork();
	if (vars->pid == -1)
	{
		close_pipe_fds(vars->pipefd);
		if (vars->prev_fd != -1)
			close(vars->prev_fd);
		set_child_running(mini, 0);
		perror("fork");
		mini->exit_status = 1;
		return (-2);
	}
	if (vars->pid == 0)
	{
		if (pids)
			free(pids);
		child_env = convert_env_to_array(mini->env_list);
		handle_child_process2(mini, vars, child_env);
		free_env_array_2(child_env);
	}
	else
		pids[vars->i] = vars->pid;
	return (execute_parent_process(vars, vars->i == mini->pipex_count));
}

static void	execute_commands_loop(t_minishell *mini, t_exec_vars *vars,
	pid_t *pids)
{
	while (vars->i <= mini->pipex_count)
	{
		if (prepare_pipe_and_sigint(mini, vars))
		{
			cleanup_pipe_fds(vars);
			return ;
		}
		vars->prev_fd = handle_fork_and_execute(mini, vars, pids);
		if (vars->prev_fd == -2)
			return ;
		vars->i++;
	}
}

void	execute_loop(t_minishell *mini, char **envp, pid_t *pids)
{
	t_exec_vars	vars;

	(void) envp;
	vars.prev_fd = -1;
	vars.pipefd[0] = -1;
	vars.pipefd[1] = -1;
	vars.i = 0;
	process_heredocs(mini);
	execute_commands_loop(mini, &vars, pids);
	cleanup_pipe_fds(&vars);
	close_all_heredoc_fds(mini);
}
