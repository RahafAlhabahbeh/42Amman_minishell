/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_loop.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rahaf <rahaf@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 00:00:00 by dal-mahr          #+#    #+#             */
/*   Updated: 2025/08/17 00:00:00 by rahaf            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

static int	prepare_pipe_and_sigint(t_minishell *mini, t_exec_vars *vars)
{
	struct sigaction	sa;

	if (vars->i < mini->pipex_count && pipe(vars->pipefd) == -1)
	{
		if (vars->prev_fd != -1)
			close(vars->prev_fd);
		perror("pipe");
		mini->exit_status = 1;
		return (1);
	}
	sa.sa_handler = SIG_IGN;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, NULL);
	vars->cmd = &mini->cmd[vars->i];
	return (0);
}

static int	handle_parent_and_fork(t_minishell *mini, t_exec_vars *vars,
	pid_t *pids)
{
	if (handle_parent_builtin(mini, vars, pids))
	{
		vars->prev_fd = execute_parent_process(vars,
				vars->i == mini->pipex_count);
		return (1);
	}
	vars->pid = fork();
	if (vars->pid == -1)
	{
		close_pipe_fds(vars->pipefd);
		if (vars->prev_fd != -1)
			close(vars->prev_fd);
		set_child_running(0);
		perror("fork");
		mini->exit_status = 1;
		return (1);
	}
	return (0);
}

static int	handle_child_or_parent(t_minishell *mini, t_exec_vars *vars,
	pid_t *pids)
{
	if (vars->pid == 0)
		handle_child_process2(mini, vars, mini->envp);
	else
	{
		pids[vars->i] = vars->pid;
		return (execute_parent_process(vars,
				vars->i == mini->pipex_count));
	}
	return (vars->prev_fd);
}

void	execute_loop(t_minishell *mini, char **envp, pid_t *pids)
{
	t_exec_vars	vars;

	(void) envp;
	vars.prev_fd = -1;
	vars.i = 0;
	process_heredocs(mini);
	while (vars.i <= mini->pipex_count)
	{
		if (prepare_pipe_and_sigint(mini, &vars))
			return ;
		if (handle_parent_and_fork(mini, &vars, pids))
		{
			vars.i++;
			continue ;
		}
		vars.prev_fd = handle_child_or_parent(mini, &vars, pids);
		vars.i++;
	}
	if (vars.prev_fd != -1)
		close(vars.prev_fd);
	close_all_heredoc_fds(mini);
}
