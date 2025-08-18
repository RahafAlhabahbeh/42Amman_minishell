/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   multiple_command_execution.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rahaf <rahaf@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 08:55:21 by dal-mahr          #+#    #+#             */
/*   Updated: 2025/08/19 00:57:40 by rahaf            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

static void	init_pids_array(pid_t *pids, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		pids[i] = -1;
		i++;
	}
}

static void	handle_single_process(t_minishell *mini, pid_t pid, int *status,
	int is_last)
{
	struct sigaction	sa;
	int					wait_result;

	if (pid <= 0)
		return ;
	ft_memset(&sa, 0, sizeof(sa));
	sa.sa_handler = SIG_IGN;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, NULL);
	wait_result = waitpid(pid, status, 0);
	if (wait_result > 0 && is_last)
	{
		if (WIFEXITED(*status))
			mini->exit_status = WEXITSTATUS(*status);
		else if (WIFSIGNALED(*status))
			mini->exit_status = 128 + WTERMSIG(*status);
	}
}

static void	wait_for_processes(t_minishell *mini, pid_t *pids, int count,
	int last_was_parent_builtin)
{
	int					status;
	int					i;

	i = 0;
	status = 0;
	while (i < count)
	{
		if (pids[i] > 0)
			handle_single_process(mini, pids[i], &status,
				(!last_was_parent_builtin || i == count - 1));
		else if (pids[i] == -2 || pids[i] == 0)
		{
			i++;
			continue ;
		}
		i++;
	}
	if (WTERMSIG(status) == SIGINT)
		write(1, "\n", 1);
	setup_signals();
}

static pid_t	*prepare_execution(t_minishell *mini, char **envp, int *count,
	int *last_cmd_builtin)
{
	pid_t	*pids;

	*count = mini->pipex_count + 1;
	pids = malloc(sizeof(pid_t) * (*count));
	if (!pids)
	{
		mini->exit_status = 1;
		return (NULL);
	}
	*last_cmd_builtin = 0;
	init_pids_array(pids, *count);
	set_child_running(1);
	execute_loop(mini, envp, pids);
	return (pids);
}

void	multiple_command_execution(t_minishell *mini, char **envp)
{
	int		count;
	pid_t	*pids;
	int		last_command_was_parent_builtin;

	pids = prepare_execution(mini, envp, &count,
			&last_command_was_parent_builtin);
	if (!pids)
		return ;
	if (pids && pids[count - 1] == -2)
		last_command_was_parent_builtin = 1;
	if (pids)
		wait_for_processes(mini, pids, count, last_command_was_parent_builtin);
	set_child_running(0);
	cleanup_heredoc_files(mini);
	if (pids)
	{
		free(pids);
		pids = NULL;
	}
}
