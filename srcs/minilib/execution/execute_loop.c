/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_loop.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rahaf <rahaf@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 00:00:00 by dal-mahr          #+#    #+#             */
/*   Updated: 2025/08/20 04:16:09 by rahaf            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

static int	handle_edge_cases(t_minishell *mini, t_exec_vars *vars)
{
	// Handle cases where we have redirections but no actual command
	if (!vars->cmd->argv || !vars->cmd->argv[0] || vars->cmd->argv[0][0] == '\0')
	{
		if (vars->cmd->input_file_name || vars->cmd->output_file_name)
		{
			// Always process redirections to check for errors
			int redir_result = handle_redirections(vars->cmd, vars->prev_fd, vars->pipefd,
					vars->i == mini->pipex_count);
			if (redir_result < 0)
			{
				mini->exit_status = 1;
			}
			else
			{
				mini->exit_status = 0;
			}
			return (1); // Successfully handled, skip to next command
		}
	}
	return (0);
}

static int	prepare_pipe_and_sigint(t_minishell *mini, t_exec_vars *vars)
{
	struct sigaction	sa;

	if (check_sigint_received())
	{
		mini->exit_status = 130;
		return (1);
	}
	vars->cmd = &mini->cmd[vars->i];
	
	// Always process redirections if there are any, even without commands
	if ((!vars->cmd->argv || !vars->cmd->argv[0] || vars->cmd->argv[0][0] == '\0') &&
		(vars->cmd->input_file_name || vars->cmd->output_file_name))
	{
		// Process redirections to check for errors
		int redir_result = handle_redirections(vars->cmd, vars->prev_fd, vars->pipefd,
				vars->i == mini->pipex_count);
		if (redir_result < 0)
		{
			mini->exit_status = 1;
		}
		else
		{
			mini->exit_status = 0;
		}
		return (1); // Skip to next command
	}
	
	if (handle_edge_cases(mini, vars))
		return (1);
	if (vars->i < mini->pipex_count)
	{
		if (pipe(vars->pipefd) == -1)
		{
			perror("pipe");
			mini->exit_status = 1;
			return (1);
		}
	}
	sa.sa_handler = SIG_IGN;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, NULL);
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
	{
		char	**child_env;

		if (pids)
			free(pids);
		child_env = convert_env_to_array(mini->env_list);
		handle_child_process2(mini, vars, child_env);
		free_env_array_2(child_env);
	}
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
	vars.pipefd[0] = -1;
	vars.pipefd[1] = -1;
	vars.i = 0;
	process_heredocs(mini);
	while (vars.i <= mini->pipex_count)
	{
		if (prepare_pipe_and_sigint(mini, &vars))
		{
			if (vars.prev_fd != -1)
				close(vars.prev_fd);
			if (vars.pipefd[0] != -1)
				close(vars.pipefd[0]);
			if (vars.pipefd[1] != -1)
				close(vars.pipefd[1]);
			return ;
		}
		if (handle_parent_and_fork(mini, &vars, pids))
		{
			if (vars.prev_fd != -1)
				close(vars.prev_fd);
			vars.i++;
			continue ;
		}
		vars.prev_fd = handle_child_or_parent(mini, &vars, pids);
		vars.i++;
	}
	if (vars.prev_fd != -1)
		close(vars.prev_fd);
	if (vars.pipefd[0] != -1)
		close(vars.pipefd[0]);
	if (vars.pipefd[1] != -1)
		close(vars.pipefd[1]);
	close_all_heredoc_fds(mini);
}
